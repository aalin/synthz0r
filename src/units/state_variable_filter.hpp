#ifndef UNITS__STATE_VARIABLE_FILTER_HPP
#define UNITS__STATE_VARIABLE_FILTER_HPP

// https://musicdsp.org/en/latest/Filters/23-state-variable.html

namespace Units {
struct StateVariableFilter {
	enum Type : int {
		LOWPASS = 0,
		HIGHPASS = 1,
		BANDPASS = 2,
		NOTCH = 3
	};

	int _cutoffHz = 1000;
	int _resonance = 100;
	int _bandwidth = 1000;

	union {
		Type _type;
		int _typeInt;
	};

	struct {
		float low = 0.0;
		float high = 0.0;
		float band = 0.0;
		float notch = 0.0;
	} state;

	float update(const Timer &timer, float input) {
		float cutoffHz = _cutoffHz / 10.0;
		float resonance = _resonance / 10.0;
		//float bandwidth = _bandwidth / 10.0;

		float f = 2.0 * std::sin(M_PI * cutoffHz / timer.sampleRate());
		//float q = Utils::clamp(resonance / bandwidth, 0.0f, 1.0f);
		float q = std::sqrt(1.0f - std::atan(std::sqrt(resonance)) * 2.0 / M_PI);
		float scale = std::sqrt(q);

		state.low = state.low + f * state.band;
		state.high = scale * input - state.low - q * state.band;
		state.band = f * state.high + state.band;
		state.notch = state.high + state.low;

		switch (_type) {
			case Type::LOWPASS:
				return state.low;
			case Type::HIGHPASS:
				return state.high;
			case Type::BANDPASS:
				return state.band;
			case Type::NOTCH:
				return state.notch;
			default:
				return input;
		}
	}
};
}

#endif
