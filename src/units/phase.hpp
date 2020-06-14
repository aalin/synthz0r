#ifndef UNITS__PHASE_HPP
#define UNITS__PHASE_HPP

namespace Units {
class Phase {
	public:
		Phase() : _phase(0.0) { }

		float update(float frequency, float sampleRate) {
			float oldPhase = _phase;

			_phase += frequency / sampleRate;

			while(_phase >= 1.0) {
				_phase -= 1.0;
			}

			while(_phase < 0.0) {
				_phase += 1.0;
			}

			return oldPhase;
		}

	private:
		float _phase = 0.0;
};
}

#endif
