#ifndef DISTORTION_HPP
#define DISTORTION_HPP

#include "base_device.hpp"
#include "../utils.hpp"

namespace Devices {
class Overdrive : public BaseDevice {
	public:
		Overdrive(float gain = 1.0, float volume = 1.0)
		: BaseDevice("Overdrive"),
		  _gain(gain),
		  _volume(volume)
		{ }

		void input(const Timer &timer, const float &value) {
			output(timer, apply(value));
		}

		void input(const Timer &timer, const StereoSample &sample) {
			output(
				timer,
				StereoSample(
					apply(sample.left),
					apply(sample.right)
				)
			);
		}

	private:
		float _gain;
		float _volume;

		float apply(const float &value) {
			return Utils::clamp(value * _gain, -1.0f, 1.0f) * _volume;
		}
};
};

#endif
