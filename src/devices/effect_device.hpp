#ifndef DEVICES__EFFECT_DEVICE_HPP
#define DEVICES__EFFECT_DEVICE_HPP

#include "base_device.hpp"
#include "../stereo_sample.hpp"
#include "../note_event.hpp"

namespace Devices {
	class EffectDevice : public BaseDevice {
		public:
			EffectDevice(std::string name) : BaseDevice(name) {}

			virtual ~EffectDevice() {}

			virtual StereoSample apply(const Timer &timer, const StereoSample &stereoSample, const NoteEventList &) {
				return apply(timer, stereoSample);
			}

			virtual StereoSample apply(const Timer &, const StereoSample &) {
				return StereoSample();
			}
	};

	typedef std::shared_ptr<EffectDevice> EffectDevicePtr;
}

#endif
