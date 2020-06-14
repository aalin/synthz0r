#ifndef ADSR_HPP
#define ADSR_HPP

#include "../utils.hpp"
#include "../timer.hpp"

class ADSR {
	public:
		bool isNoteDone(const Timer &timer, float noteOffTime) {
			const float seconds = timer.seconds();

			if (noteOffTime >= 0.0 && seconds >= noteOffTime) {
				return seconds > noteOffTime + release();
			}

			return false;
		}

		float update(const Timer &timer, float noteOnTime, float noteOffTime) {
			const float time = timer.seconds();

			if (noteOffTime >= 0.0 && time >= noteOffTime) {
				const float dt = time - noteOffTime;

				if (dt > release()) {
					return 0.0;
				}

				return Utils::lerp(sustain(), 0.0f, dt / release());
			}

			const float dt = time - noteOnTime;

			if (dt < attack()) {
				return Utils::lerp(0.0f, 1.0f, dt / attack());
			}

			if (dt < attack() + decay()) {
				return Utils::lerp(1.0f, sustain(), (dt - attack()) / decay());
			}

			return sustain();
		}

		int _attackMs, _decayMs, _releaseMs, _sustain;

		float attack() { return _attackMs / 1000.0; }
		float decay() { return _decayMs / 1000.0; }
		float release() { return _releaseMs / 1000.0; }
		float sustain() { return _sustain / 127.0; }
};

#endif
