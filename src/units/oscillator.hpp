#ifndef UNITS__OSCILLATOR_HPP
#define UNITS__OSCILLATOR_HPP

#include <cmath>
#include "./phase.hpp"

namespace Units {
class Oscillator {
	public:
		enum Type {
			SINE,
			SAW,
			TRIANGLE,
			SQUARE,
			NOISE
		};

		Oscillator(Type type = Type::SQUARE)
		: _type(type)
		{}

		float update(float frequency, const Timer &timer) {
			float sampleRate = timer.sampleRate();

			switch (_type) {
				case SINE:
					return sine(frequency, sampleRate);
				case SAW:
					return saw(frequency, sampleRate);
				case TRIANGLE:
					return triangle(frequency, sampleRate);
				case SQUARE:
					return square(frequency, sampleRate);
				case NOISE:
					return noise(frequency, sampleRate);
				default:
					return 0.0;
			}
		}


	private:
		float noise(float frequency, float sampleRate) {
			updatePhase(frequency, sampleRate);
			constexpr float randMax = RAND_MAX;
			return (rand() / randMax) * 2.0 - 1.0;
		}

		float sine(float frequency, float sampleRate) {
			return std::sin(updatePhase(frequency, sampleRate));
		}

		float saw(float frequency, float sampleRate) {
			float phase = updatePhase(frequency, sampleRate) / M_PI;
			return (phase - std::floor(phase)) * 2.0 - 1.0;
		}

		float triangle(float frequency, float sampleRate) {
			constexpr float HALF_PI = M_PI / 2.0;
			return std::asin(std::cos(updatePhase(frequency, sampleRate))) / HALF_PI;
		}

		float square(float frequency, float sampleRate) {
			if (sine(frequency, sampleRate) < 0.0) {
				return -1.0;
			} else {
				return 1.0;
			}
		}

		float updatePhase(float frequency, float sampleRate) {
			constexpr float TWO_PI = M_PI * 2.0;

			return _phase.update(frequency, sampleRate) * TWO_PI;
		}

		Type _type;
		Phase _phase;
};
}

#endif
