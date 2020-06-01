#ifndef EFFECT_HPP
#define EFFECT_HPP

namespace Effects {
class BaseEffect {
	public:
		virtual float apply(float value) {
			return value;
		}
};
}

#endif
