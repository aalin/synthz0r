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
		  _sampleRate(sampleRate),
		  _markerLeft(0),
		  _markerRight(240 * 4 * 4),
		  _loop(false)
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
					if (static_cast<uint32_t>(_position.totalTicks()) == _markerRight) {
						_position.skipTo(_markerLeft);
					}

					std::cout << _position << std::endl;
				}
			}
		}

		bool shouldTriggerNotes() const {
			return _state == State::PLAYING && _position.ticksChanged();
		}

		void setMarkerLeft(uint32_t position) {
			_markerLeft = position;
		}

		void setMarkerRight(uint32_t position) {
			_markerRight = position;
		}

		void setLoop(bool value) {
			_loop = value;
		}

		bool isLooping() {
			return _loop;
		}

	private:
		State _state;
		Position _position;
		Timer _timer;
		float _bpm;
		const unsigned int _sampleRate;
		uint32_t _markerLeft;
		uint32_t _markerRight;
		bool _loop;
};

#endif
