#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
	public:
		Timer(float sampleRate)
		: _tick(0),
		  _seconds(0.0),
		  _sampleRate(sampleRate)
		{}

		float getSampleRate() const {
			return _sampleRate;
		}

		float getSeconds() const {
			return _seconds;
		}

		void setSeconds(float seconds) {
			_tick = seconds * _sampleRate;
			_seconds = seconds;
		}

		void tick() {
			_tick++;
			_seconds = _tick / _sampleRate;
		}

		void reset() {
			_tick = 0;
		}

	private:
		unsigned long int _tick;
		float _seconds;
		const float _sampleRate;
};

#endif
