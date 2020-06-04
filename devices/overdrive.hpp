#ifndef DISTORTION_HPP
#define DISTORTION_HPP

#include "base_device.hpp"
#include "../utils.hpp"

namespace Devices {
class Overdrive : public BaseDevice {
	public:
		Overdrive(int gain = 50, int volume = 100)
		: BaseDevice("Overdrive", {
			Parameter("gain", 0, 100, gain, _gain),
			Parameter("volume", 0, 128, volume, _volume),
		  })
		{}

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
		int _gain;
		int _volume;

		float gain() {
			return _gain / 10.f;
		}

		float volume() {
			return _volume / 100.f;
		}

		float apply(const float &value) {
			return Utils::clamp(value * gain(), -1.0f, 1.0f) * volume();
		}
};
};

#endif