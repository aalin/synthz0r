#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
	public:
		Timer(float sampleRate)
		: _tick(0),
		  _seconds(0.0),
		  _sampleRate(sampleRate)
		{}

		float sampleRate() const {
			return _sampleRate;
		}

		float seconds() const {
			return _seconds;
		}

		void setSeconds(float seconds) {
			_tick = seconds * _sampleRate;
			_seconds = seconds;
		}

		size_t tick() const {
			return _tick;
		}

		void update() {
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
