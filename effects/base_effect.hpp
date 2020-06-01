#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <memory>

namespace Effects {
class BaseEffect {
	public:
		virtual float apply(float value) {
			return value;
		}
};

typedef std::shared_ptr<BaseEffect> EffectPtr;
};

#endif
