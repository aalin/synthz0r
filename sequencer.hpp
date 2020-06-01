#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "synth.hpp"

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

		void update(std::shared_ptr<Synth> synth, float time) {
			const size_t step = Utils::mod(static_cast<size_t>(time * _speed), _notes.size());

			if (step != _currentStep) {
				const int oldNote = _notes[Utils::mod(step - 1, _notes.size())];

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
		int _currentStep;
		float _speed;
		float _velocity;
};

#endif
