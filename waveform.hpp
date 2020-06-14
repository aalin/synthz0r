#ifndef WAVEFORM_HPP
#define WAVEFORM_HPP

#include <cmath>
#include <vector>
#include <string>
#include "utils.hpp"

struct Waveform {
	static const std::vector<Waveform> WAVEFORMS;

	const std::string name;
	const unsigned int sampleRate;
	const std::vector<int16_t> data;

	size_t size() const {
		return data.size();
	}

	float getValue(float index) const {
		float index2 = index * size();
		const int i = static_cast<int>(index2);
		const float f = index2 - i;

		return Utils::lerp(
			static_cast<float>(data[(i + 0) % data.size()]),
			static_cast<float>(data[(i + 1) % data.size()]),
			f
		) / 32767.0;
	}
};

#endif
