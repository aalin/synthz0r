#ifndef UNITS__ADSR2_HPP
#define UNITS__ADSR2_HPP

#include "../utils.hpp"
#include "../timer.hpp"

namespace Units {
	class ADSR2 {
		public:
			struct Settings {
				int _attackStart = 0;
				int _attackMs    = 100;
				int _decayMs     = 100;
				int _releaseMs   = 100;
				int _sustain     = 50;

				float attackStart() const { return _attackStart / 100.0; }
				float attack()      const { return _attackMs    / 1000.0; }
				float decay()       const { return _decayMs     / 1000.0; }
				float release()     const { return _releaseMs   / 1000.0; }
				float sustain()     const { return _sustain     / 100.0; }
			};

			enum State {
				ATTACKING  = 0,
				DECAYING   = 1,
				SUSTAINING = 2,
				RELEASING  = 3,
				OFF        = 4
			};

			ADSR2(const Settings settings)
			: _settings(settings),
			  _state(State::ATTACKING)
			{}

			bool isOff() {
				return _state == State::OFF;
			}

			float update(const Timer &timer, float noteOnTime, float noteOffTime);

		private:
			const Settings _settings;
			State _state;
	};
}

#endif
