#ifndef STEREO_SAMPLE_HPP
#define STEREO_SAMPLE_HPP

#include <iostream>

struct StereoSample {
	StereoSample() {}

	StereoSample(float mono)
	: left(mono),
	  right(mono)
	{}

	StereoSample(float left, float right)
	: left(left),
	  right(right)
	{}

	StereoSample(const StereoSample &other)
	: left(other.left),
	  right(other.right)
	{}

	StereoSample & reset() {
		left = 0.0;
		right = 0.0;
		return *this;
	}

	StereoSample & add(const StereoSample &other) {
		left += other.left;
		right += other.right;
		return *this;
	}

	StereoSample & add(const float &value) {
		left += value;
		right += value;
		return *this;
	}

	StereoSample & sub(const StereoSample &other) {
		left -= other.left;
		right -= other.right;
		return *this;
	}

	StereoSample & sub(const float &value) {
		left -= value;
		right -= value;
		return *this;
	}

	constexpr StereoSample& operator=(const StereoSample &other) {
		left = other.left;
		right = other.right;
		return *this;
	}

	float mono() const {
		return (left + right) / 2.0;
	}

	union {
		float left, l = 0.0;
	};

	union {
		float right, r = 0.0;
	};
};

std::ostream & operator<<(std::ostream &out, const StereoSample &sample);

#endif
