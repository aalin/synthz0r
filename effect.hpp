#ifndef EFFECT_HPP
#define EFFECT_HPP

class Effect {
	public:
		virtual float apply(float value) {
			return value;
		}
};

#endif
