#ifndef DEVICES__EFFECTS__BITCRUSHER_HPP
#define DEVICES__EFFECTS__BITCRUSHER_HPP

#include "../effect_device.hpp"
#include "../../utils.hpp"

namespace Devices {
	namespace Effects {
		class Bitcrusher : public Devices::EffectDevice {
			public:
				Bitcrusher(int bits = 4, int volume = 100) : EffectDevice("Bitcrusher") {
					setupParameters({
						Parameter("bits",   0, 100, bits,   _bits),
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
				int _bits;
				int _volume;

				float volume() const {
					return _volume / 100.f;
				}

				float apply(const float &value) {
					float max = pow(_bits, 2);
					int clamped = Utils::clamp(value * max, -max, max);
					return (clamped / max) * volume();
				}
		};
	}
}

#endif
