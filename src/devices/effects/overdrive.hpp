#ifndef DEVICES__EFFECTS__OVERDRIVE_HPP
#define DEVICES__EFFECTS__OVERDRIVE_HPP

#include "../effect_device.hpp"
#include "../../utils.hpp"

namespace Devices {
	namespace Effects {
		class Overdrive : public EffectDevice {
			public:
				Overdrive(int gain = 50, int volume = 100)
				: EffectDevice("Overdrive") {
					setupParameters({
						Parameter("gain",   0, 100, gain,   _gain),
						Parameter("volume", 0, 128, volume, _volume),
					});
				}

				StereoSample apply(const Timer &, const StereoSample &sample) {
					return StereoSample(
						apply(sample.left),
						apply(sample.right)
					);
				}

			private:
				int _gain;
				int _volume;

				float gain() {
					return _gain / 10.f;
				}

				float volume() {
					return _volume / 100.f;
				}

				float apply(const float &value) {
					return Utils::clamp(value * gain(), -1.0f, 1.0f) * volume();
				}
		};
	}
}

#endif
