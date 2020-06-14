#ifndef PHASE_HPP
#define PHASE_HPP

namespace Units {
class Phase {
	public:
		Phase() : _phase(0.0) { }

		float update(float frequency, float sampleRate) {
			constexpr float TWO_PI = M_PI * 2.0;

			float oldPhase = _phase;

			_phase += TWO_PI * frequency / sampleRate;

			while(_phase >= TWO_PI) {
				_phase -= TWO_PI;
			}

			while(_phase < 0.0) {
				_phase += TWO_PI;
			}

			return oldPhase;
		}

	private:
		float _phase = 0.0;
};
};

#endif
