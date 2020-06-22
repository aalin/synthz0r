#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
	public:
		Timer(float sampleRate)
		: _tick(0),
		  _seconds(0.0),
		  _bpm(120.0),
		  _beat(0.0),
		  _sampleRate(sampleRate)
		{}

		float sampleRate() const {
			return _sampleRate;
		}

		float seconds() const {
			return _seconds;
		}

		float bpm() const {
			return _bpm;
		}

		double beat() const {
			return _beat;
		}

		double beatsPerSample() const {
			const double beatsPerSecond = _bpm / 60.0;
			return beatsPerSecond / _sampleRate;
		}

		void setSeconds(float seconds) {
			_tick = seconds * _sampleRate;
			_seconds = seconds;
		}

		size_t tick() const {
			return _tick;
		}

		void update() {
			_beat += beatsPerSample();
			_seconds = _tick++ / _sampleRate;
		}

		void reset() {
			_tick = 0;
		}

	private:
		unsigned long int _tick;
		float _seconds;
		float _bpm;
		double _beat;
		const float _sampleRate;
};

#endif
