#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <memory>
#include "../stereo_sample.hpp"

namespace Effects {
class BaseEffect {
	public:
		virtual float apply(float value) {
			return value;
		}

		virtual StereoSample apply(const StereoSample &sample) {
			return sample;
		}
};

typedef std::shared_ptr<BaseEffect> EffectPtr;
};

#endif
