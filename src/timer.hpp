#ifndef TIMER_HPP
#define TIMER_HPP

#include "position.hpp"

class Timer {
	public:
		Timer(float sampleRate)
		: _tick(0),
		  _seconds(0.0),
		  _bpm(120.0),
		  _sampleRate(sampleRate)
		{}

		float bpm() const {
			return _bpm;
		}

		float sampleRate() const {
			return _sampleRate;
		}

		float seconds() const {
			return _seconds;
		}

		const Position & position() const {
			return _position;
		}

		void setSeconds(float seconds) {
			_tick = seconds * _sampleRate;
			_seconds = seconds;
		}

		size_t tick() const {
			return _tick;
		}

		void update() {
			_position.update(_bpm, _sampleRate);
			_seconds = _tick++ / _sampleRate;
		}

	private:
		unsigned long int _tick;
		float _seconds;
		float _bpm;
		const float _sampleRate;
		Position _position;
};

#endif
