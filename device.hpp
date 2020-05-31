#ifndef DEVICE_HPP
#define DEVICE_HPP

class Device {
	public:
		Device()
		: _time(0.0)
		{ }

		virtual float tick(float time, float sampleRate) {
			_time = time;
		}

		virtual float update(float time, float sampleRate, float pitchBend = 0.0) {
			return 0.0;
		}

	private:
		float _time;
};

#endif
