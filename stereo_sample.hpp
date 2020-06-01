#ifndef STEREO_SAMPLE_HPP
#define STEREO_SAMPLE_HPP

struct StereoSample {
	StereoSample()
	: left(0.0),
	  right(0.0)
	{}

	StereoSample(const StereoSample &other) {
		left = other.left;
		right = other.right;
	}

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

	float monoSample() const {
		return (left + right) / 2.0;
	}

	float left;
	float right;
};

std::ostream & operator<<(std::ostream &out, const StereoSample &sample) {
	out << "StereoSample(" << sample.left << ", " << sample.right << ")";
	return out;
}

#endif