#ifndef UTILS_HPP
#define UTILS_HPP

#include <type_traits>

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
};

#endif