#ifndef UNITS__ADSR_HPP
#define UNITS__ADSR_HPP

#include "../utils.hpp"
#include "../timer.hpp"

namespace Units {
class ADSR {
	public:
		bool isNoteDone(const Timer &timer, float noteOffTime) {
			const float seconds = timer.seconds();

			if (noteOffTime >= 0.0 && seconds >= noteOffTime) {
				return seconds > noteOffTime + release();
			}

			return false;
		}

		float update(const Timer &timer, float noteOnTime, float noteOffTime);

		int _attackStart = 0;
		int _attackMs = 100;
		int _decayMs = 100;
		int _releaseMs = 100;
		int _sustain = 50;

		float attackStart() { return _attackStart / 100.0; }
		float attack() { return _attackMs / 1000.0; }
		float decay() { return _decayMs / 1000.0; }
		float release() { return _releaseMs / 1000.0; }
		float sustain() { return _sustain / 127.0; }
};
}

#endif
