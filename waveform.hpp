#ifndef WAVEFORM_HPP
#define WAVEFORM_HPP

#include <cmath>
#include <vector>
#include <string>
#include "utils.hpp"

struct Waveform {
	static std::vector<Waveform> WAVEFORMS;
	const std::string name;
	const unsigned int sampleRate;
	const std::vector<int16_t> data;

	float getValue(float index) {
		int16_t i = index;
		float f = index - i;

		return Utils::lerp(
			static_cast<float>(data[i]),
			static_cast<float>(data[(i + 1) % data.size()]),
			f
		) / 0x7fff;
	}
};

#endif
