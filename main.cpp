#include <iostream>
#include <memory>
#include <vector>
#include "engine.hpp"
#include "pulse_audio.hpp"
#include "synth.hpp"
#include "engine.hpp"

constexpr unsigned int BUFFER_SIZE = 1024;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;

template<typename T>
T mod(T n, T m) {
  if (n < 0) {
    return ((n % m) + m) % m;
  }

  return n % m;
}

class Sequencer {
	public:
		Sequencer(size_t steps, float speed, float velocity) {
			_speed = speed;
			_velocity = velocity;
			_currentStep = 0;
			_notes.resize(steps, -1);
		}

		Sequencer & setStep(size_t step, int value) {
			_notes[step] = value;
			return *this;
		}

		Sequencer & clearStep(size_t step) {
			_notes[step] = 0;
			return *this;
		}

		Sequencer & setSpeed(float speed) {
			_speed = speed;
			return *this;
		}

		Sequencer & setVelocity(float velocity) {
			_velocity = velocity;
			return *this;
		}

		void update(std::shared_ptr<Synth> synth, float time) {
			const size_t step = static_cast<size_t>(time * _speed) % _notes.size();

			if (step != _currentStep) {
				const int oldNote = _notes[mod(step - 1, _notes.size())];

				if (oldNote != -1) {
					synth->noteOff(oldNote);
				}

				if (_notes[step] != -1) {
					synth->noteOn(_notes[step], _velocity);
				}

				_currentStep = step;
			}
		}

	private:
		std::vector<int> _notes;
		unsigned int _currentStep;
		float _speed;
		float _velocity;
};

int main(int argc, char *argv[]) {
	try {
		Engine<int16_t> engine(argv[0], SAMPLE_RATE, NUM_CHANNELS, BUFFER_SIZE);

		engine.start();

		auto synth1 = std::make_shared<Synth>(Oscillator::Type::SAW);
		synth1->amplitude = 0.8;
		synth1->transpose = 0;
		synth1->envelope
			.setAttack(0.25)
			.setDecay(0.25)
			.setSustain(0.5)
			.setRelease(0.05);
		engine.addDevice(synth1);

		auto synth2 = std::make_shared<Synth>(Oscillator::Type::SQUARE);
		synth2->amplitude = 0.4;
		synth2->transpose = 0;
		synth2->envelope
			.setAttack(0.25)
			.setDecay(0.25)
			.setSustain(0.5)
			.setRelease(0.05);
		engine.addDevice(synth2);

		Sequencer sequencer1(8, 1.0, 1.0);
		sequencer1
			.setStep(0, 52)
			.setStep(1, 55)
			.setStep(2, 57)
			.setStep(3, 55)
			.setStep(4, 62)
			.setStep(5, 60)
			.setStep(6, 62)
			.setStep(7, 64);

		Sequencer sequencer2(8, 1.0, 1.0);
		sequencer2
			.setStep(0, 52)
			.setStep(1, 55)
			.setStep(2, 57)
			.setStep(3, 55)
			.setStep(4, 62)
			.setStep(5, 60)
			.setStep(6, 62)
			.setStep(7, 64);

		while (engine.running()) {
			const float time = engine.getScaledTime();
			std::cout << "Time: " << time << std::endl;

			sequencer1.setSpeed(8.0 + std::sin(time / 4.0) / 2.0);
			sequencer1.update(synth1, time);
			sequencer2.setSpeed(7.0 + std::sin(time / 3.0) / 2.0);
			sequencer2.update(synth2, time);

			engine.update();
		}
	} catch (const char *msg) {
		std::cerr << "Error: " << msg << std::endl;
	}

	return 0;
}
