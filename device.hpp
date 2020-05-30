#ifndef DEVICE_HPP
#define DEVICE_HPP

class Device {
	public:
		virtual float update(float time, float sampleRate) {
			return 0.0;
		}
};

#endif
