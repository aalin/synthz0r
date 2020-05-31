#ifndef ADSR_HPP
#define ADSR_HPP

#include "utils.hpp"

class ADSR {
	public:
		ADSR()
		: _attack(0.1),
		  _decay(0.1),
		  _sustain(0.2),
		  _release(0.2)
		{}

		ADSR(float attack, float decay, float sustain, float release)
		: _attack(attack),
		  _decay(decay),
		  _sustain(sustain),
		  _release(release)
		{}

		ADSR & setAttack(float attack) {
			_attack = attack;
			return *this;
		}

		ADSR & setDecay(float decay) {
			_decay = decay;
			return *this;
		}

		ADSR & setSustain(float sustain) {
			_sustain = sustain;
			return *this;
		}

		ADSR & setRelease(float release) {
			_release = release;
			return *this;
		}

		bool isNoteDone(float time, float noteOffTime) {
			if (noteOffTime >= 0.0 && time >= noteOffTime) {
				return time > noteOffTime +_release;
			}

			return false;
		}

		float getValue(float time, float noteOnTime, float noteOffTime) {
			if (noteOffTime >= 0.0 && time >= noteOffTime) {
				const float dt = time - noteOffTime;

				if (dt > _release) {
					return 0.0;
				}

				return Utils::lerp(_sustain, 0.0f, dt / _release);
			}

			const float dt = time - noteOnTime;

			if (dt < _attack) {
				return Utils::lerp(0.0f, 1.0f, dt / _attack);
			}

			if (dt < _attack + _decay) {
				return Utils::lerp(1.0f, _sustain, (dt - _attack) / _decay);
			}

			return _sustain;
		}

	private:
		float _attack;
		float _decay;
		float _sustain;
		float _release;
};

#endif
