#ifndef UTILS_HPP
#define UTILS_HPP

namespace Utils {
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

	template<typename T> T clip(T value) {
		return value;
	}

	template<typename T, typename V> T clip(V value) {
		constexpr T min = std::numeric_limits<T>::min();
		constexpr T max = std::numeric_limits<T>::max();

		if (value < min) {
			return min;
		}

		if (value > max) {
			return max;
		}

		return value;
	}
};

#endif
