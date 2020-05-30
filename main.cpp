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
		Sequencer(size_t steps, float speed) {
			_speed = speed;
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

		void update(std::shared_ptr<Synth> synth, float time) {
			const size_t step = static_cast<size_t>(time * _speed) % _notes.size();


			if (step != _currentStep) {
				std::cout << "Step: " << step << std::endl;
				const int oldNote = _notes[mod(step - 1, _notes.size())];

				if (oldNote != -1) {
					synth->removeVoice(oldNote);
				}

				if (_notes[step] != -1) {
					synth->addVoice(_notes[step]);
				}

				_currentStep = step;
			}
		}

	private:
		std::vector<int> _notes;
		unsigned int _currentStep;
		float _speed;
};

int main(int argc, char *argv[]) {
	try {
		Engine<int16_t> engine(argv[0], SAMPLE_RATE, NUM_CHANNELS, BUFFER_SIZE);

		engine.start();

		auto synth = std::make_shared<Synth>();

		//synth->addVoice(69);
		//synth->addVoice(69 + 4);
		//synth->addVoice(69 + 7);

		engine.addDevice(synth);

		Sequencer sequencer(8, 8.0);

		sequencer
			.setStep(0, 52 + 24)
			.setStep(1, 55 + 24)
			.setStep(2, 57 + 24)
			.setStep(3, 55 + 24)
			.setStep(4, 62 + 24)
			.setStep(5, 60 + 24)
			.setStep(6, 62 + 24)
			.setStep(7, 64 + 24);

		while (engine.running()) {
			const float time = engine.getTime();

			sequencer.setSpeed(8.0 + std::sin(time / 4.0) / 2.0);
			sequencer.update(synth, time);

			// std::cout << "Time: " << time << std::endl;
			engine.update();
		}
	} catch (const char *msg) {
		std::cerr << "Error: " << msg << std::endl;
	}

	return 0;
}
