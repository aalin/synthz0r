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

			virtual StereoSample apply(const Transport &transport, const StereoSample &stereoSample, const NoteEventList &) {
				return apply(transport, stereoSample);
			}

			virtual StereoSample apply(const Transport &, const StereoSample &) {
				return StereoSample();
			}
	};

	typedef std::shared_ptr<EffectDevice> EffectDevicePtr;
}

#endif
