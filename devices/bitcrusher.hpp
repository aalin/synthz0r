#ifndef BITCRUSHER_HPP
#define BITCRUSHER_HPP

#include "base_device.hpp"
#include "../utils.hpp"

namespace Devices {
class Bitcrusher : public BaseDevice {
	public:
		Bitcrusher(uint8_t bits = 2, float volume = 1.0)
		: BaseDevice("Bitcrusher"),
		  _bits(bits),
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
		uint8_t _bits;
		float _volume;

		float apply(const float &value) {
			float max = pow(_bits, 2);
			int clamped = Utils::clamp(value * max, -max, max);
			return (clamped / max) * _volume;
		}
};
}

#endif
