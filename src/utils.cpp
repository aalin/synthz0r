#include <cmath>
#include "utils.hpp"

StereoSample Utils::pan(float value, float f) {
	constexpr const float x = std::sqrt(2.0) / 2.0;
	constexpr const float radians = M_PI / 180.0;
	const float r = f * 45.0f * radians;
	const float c = std::sin(r);
	const float s = std::cos(r);

	StereoSample out(
		value * x * (c - s),
		value * x * (c + s)
	);

	return out;
}

float Utils::noteToFrequency(float note) {
	return std::pow(2, (note - 69) / 12) * 440.0;
}

float Utils::rsin(float v, float min, float max) {
	return (std::sin(v) / 2.0 + 0.5) * (max - min) + min;
}

float Utils::rcos(float v, float min, float max) {
	return (std::cos(v) / 2.0 + 0.5) * (max - min) + min;
}
