#ifndef BITCRUSHER_HPP
#define BITCRUSHER_HPP

#include "base_effect.hpp"
#include "../utils.hpp"

namespace Effects {
class Bitcrusher : public BaseEffect {
	public:
		Bitcrusher(uint8_t bits = 2)
		: _bits(bits)
		{ }

		float apply(const float &value) {
			float max = pow(_bits, 2);
			int clamped = Utils::clamp(value * max, -max, max);
			return clamped / max;
		}

		StereoSample apply(const StereoSample &sample) {
			StereoSample out;
			out.left = apply(sample.left);
			out.right = apply(sample.right);
			return out;
		}

	private:
		uint8_t _bits;
};
}

#endif
