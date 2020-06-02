#ifndef BITCRUSHER_HPP
#define BITCRUSHER_HPP

#include "base_device.hpp"
#include "../utils.hpp"

namespace Devices {
class Bitcrusher : public BaseDevice {
	public:
		Bitcrusher(int bits = 4, int volume = 100)
		: BaseDevice("Bitcrusher", {
			Parameter("gain", 0, 100, bits, _bits),
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
		int _bits;
		int _volume;

		float volume() const {
			return _volume / 100.f;
		}

		float apply(const float &value) {
			float max = pow(_bits, 2);
			int clamped = Utils::clamp(value * max, -max, max);
			return (clamped / max) * volume();
		}
};
}

#endif
