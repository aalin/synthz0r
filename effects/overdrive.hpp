#ifndef DISTORTION_HPP
#define DISTORTION_HPP

#include "base_effect.hpp"
#include "../utils.hpp"

namespace Effects {
class Overdrive : public BaseEffect {
	public:
		Overdrive(float gain = 1.0, float volume = 1.0)
		: _gain(gain),
		  _volume(volume)
		{ }

		float apply(float value) {
			return Utils::clamp(value * _gain, -1.0f, 1.0f) * _volume;
		}

	private:
		float _gain;
		float _volume;
};
};

#endif
