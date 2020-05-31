#ifndef BITCRUSHER_HPP
#define BITCRUSHER_HPP

#include "effect.hpp"
#include "utils.hpp"

class Bitcrusher : public Effect {
	public:
		Bitcrusher(uint8_t bits = 2)
		: _bits(bits)
		{ }

		float apply(float value) {
			float max = pow(_bits, 2);
			int clamped = Utils::clamp(value * max, -max, max);
			return clamped / max;
		}

	private:
		uint8_t _bits;
};

#endif
