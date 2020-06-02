#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "devices/synth.hpp"

class Sequencer {
	public:
		Sequencer(size_t steps, float speed, float velocity) {
			_speed = speed;
			_velocity = velocity;
			_currentStep = -1;
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

		void update(const Timer &timer, std::shared_ptr<Devices::Synth> synth) {
			const int length = _notes.size();
			const int step = Utils::mod(static_cast<int>(timer.seconds() * _speed), length);

			if (step != _currentStep) {
				const int oldNote = _notes[Utils::mod(step - 1, length)];

				if (oldNote != -1) {
					synth->noteOff(timer, oldNote);
				}

				if (_notes[step] != -1) {
					synth->noteOn(timer, _notes[step], _velocity);
				}

				_currentStep = step;
			}
		}

	private:
		std::vector<int> _notes;
		int _currentStep;
		float _speed;
		float _velocity;
};

#endif
