#ifndef DISTORTION_HPP
#define DISTORTION_HPP

#include "base_effect.hpp"
#include "../utils.hpp"

namespace Effects {
class Overdrive : public BaseEffect {
	public:
		Overdrive(float gain = 1.0)
		: _gain(gain)
		{ }

		float apply(float value) {
			return Utils::clamp(value * _gain, -1.0f, 1.0f);
		}

	private:
		uint8_t _gain;
};
};

#endif
