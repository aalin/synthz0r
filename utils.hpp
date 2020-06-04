#ifndef UTILS_HPP
#define UTILS_HPP

#include <type_traits>
#include <limits>
#include "stereo_sample.hpp"

namespace Utils {
	template<typename T>
	T lerp(T a, T b, T f) {
		return a + f * (b - a);
	}

	template<typename T>
	T clamp(T value, T min, T max) {
		if (value < min) {
			return min;
		} else if (value > max) {
			return max;
		} else {
			return value;
		}
	}

	template<typename T>
	T mod(T n, T m) {
		if (n < 0) {
			return ((n % m) + m) % m;
		}

		return n % m;
	}

	template<typename T, typename V> T clip(V value) {
		constexpr T min = std::numeric_limits<T>::min();
		constexpr T max = std::numeric_limits<T>::max();

		if (std::is_same<T, int8_t>::value) {
			return ((value + 1.0) / 2.0) * 255.0;
		}

		if (value < min) {
			return min;
		}

		if (value > max) {
			return max;
		}

		return value;
	}

	inline float volume(float value) {
		return (std::exp(value) - 1) / (2.718281828459045 - 1);
	}

	StereoSample pan(float value, float f);

	float noteToFrequency(float note);

	// Ranged sin, returns a sine wave between min and max
	float rsin(float v, float min = 0.0, float max = 1.0);

	// Ranged cos, returns a cosine wave between min and max
	float rcos(float v, float min = 0.0, float max = 1.0);
};

#endif
