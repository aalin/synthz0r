#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP

#include "timer.hpp"
#include "position.hpp"
#include <cmath>

class Transport {
	public:
		enum State {
			PAUSED = 0,
			PLAYING = 1,
		};

		Transport(float sampleRate)
		: _state(PAUSED),
		  _timer(sampleRate),
		  _bpm(120.0),
		  _sampleRate(sampleRate)
		{}

		void setBpm(float bpm) {
			if (bpm < 1.0) {
				_bpm = 1.0;
			} else {
				_bpm = bpm;
			}
		}

		State state() {
			return _state;
		}

		void play() {
			_state = PLAYING;
		}

		void pause() {
			_state = PAUSED;
		}

		const Position & position() const {
			return _position;
		}

		const Timer & timer() const {
			return _timer;
		}

		float sampleRate() const {
			return _sampleRate;
		}

		float secondsElapsedSinceStart() const {
			return _timer.seconds();
		}

		unsigned long int ticksElapsedSinceStart() const {
			return _timer.ticks();
		}

		void update() {
			_timer.update(_sampleRate);

			if (_state == State::PLAYING) {
				_position.update(_bpm, _sampleRate);

				if (_position.ticksChanged() && _position.ticks == 0) {
					std::cout << _position << std::endl;
				}
			}
		}

		bool shouldTriggerNotes() const {
			return _state == State::PLAYING && _position.ticksChanged();
		}

	private:
		State _state;
		Position _position;
		Timer _timer;
		float _bpm;
		const unsigned int _sampleRate;
};

#endif
