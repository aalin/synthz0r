#include "stereo_sample.hpp"

std::ostream & operator<<(std::ostream &out, const StereoSample &sample) {
	out << "StereoSample(" << sample.left << ", " << sample.right << ")";
	return out;
}
