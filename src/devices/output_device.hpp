#ifndef OUTPUT_DEVICE_HPP
#define OUTPUT_DEVICE_HPP

#include "base_device.hpp"
#include "../stereo_sample.hpp"

namespace Devices {
class OutputDevice : public BaseDevice {
	public:
		OutputDevice() : BaseDevice("Output device") {}

		void input(const Timer &, const float &value) {
			_stereoSample.add(value);
		}

		void input(const Timer &, const StereoSample &value) {
			_stereoSample.add(value);
		}

		const StereoSample & getSample() const  {
			return _stereoSample;
		}

		void reset() {
			_stereoSample.reset();
		}

	private:
		StereoSample _stereoSample;
};
}

#endif
