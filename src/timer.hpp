#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
	public:
		Timer(float sampleRate)
		: _ticks(0),
		  _seconds(0.0),
		  _sampleRate(sampleRate)
		{}

		float seconds() const {
			return _seconds;
		}

		unsigned long int ticks() const {
			return _ticks;
		}

		void update(const float &sampleRate) {
			_seconds = _ticks++ / sampleRate;
		}

		const float & sampleRate() const {
			return _sampleRate;
		}

	private:
		unsigned long int _ticks;
		float _seconds;
		const float _sampleRate;
};

#endif
