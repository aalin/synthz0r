#ifndef ADSR_HPP
#define ADSR_HPP

#include <iostream>

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
		}

		ADSR & setDecay(float decay) {
			_decay = decay;
		}

		ADSR & setSustain(float sustain) {
			_sustain = sustain;
		}

		ADSR & setRelease(float release) {
			_release = release;
		}

		bool isNoteDone(float time, float noteOffTime) {
			if (noteOffTime >= 0.0 && time >= noteOffTime) {
				const float dt = time - noteOffTime;

				return dt > _release;
			}

			return false;
		}

		float getValue(float time, float noteOnTime, float noteOffTime) {
			if (noteOffTime >= 0.0 && time >= noteOffTime) {
				const float dt = time - noteOffTime;

				if (dt > _release) {
					return 0.0;
				}

				float v = lerp(_sustain, 0.0, dt / _release);

				return lerp(_sustain, 0.0, dt / _release);
			}

			const float dt = time - noteOnTime;

			if (dt < _attack) {
				float v = lerp(0.0, 1.0, dt / _attack);
				return lerp(0.0, 1.0, dt / _attack);
			}

			if (dt < _attack + _decay) {
				return lerp(1.0, _sustain, (dt - _attack) / _decay);
			}

			return _sustain;
		}

	private:
		float _attack;
		float _decay;
		float _sustain;
		float _release;

		float lerp(float a, float b, float f) {
			return a + f * (b - a);
		}
};

#endif
