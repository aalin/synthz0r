#ifndef OSCILLATOR_HPP
#define OSCILLATOR_HPP

#include <cmath>

class Oscillator {
	public:
		Oscillator() : _phase(0) {
		}

		float sine(float frequency, float sampleRate) {
			constexpr float TWO_PI = 2.0 * M_PI;

			_phase += TWO_PI * frequency / sampleRate;

			while(_phase >= TWO_PI) {
				_phase -= TWO_PI;
			}

			while(_phase < 0.0) {
				_phase += TWO_PI;
			}

			return std::sin(_phase);
		}

	private:
		float _phase;
};

#endif
