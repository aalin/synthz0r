#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
	public:
		Timer(float sampleRate)
		: _tick(0),
		  _sampleRate(sampleRate)
		{}

		float getSampleRate() const {
			return _sampleRate;
		}

		float getSeconds() const {
			return _tick / _sampleRate;
		}

		void setSeconds(float seconds) {
			_tick = seconds * _sampleRate;
		}

		void tick() {
			_tick++;
		}

		void reset() {
			_tick = 0;
		}

	private:
		unsigned long int _tick;
		const float _sampleRate;
};

#endif
