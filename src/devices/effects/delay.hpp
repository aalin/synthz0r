#ifndef DEVICES__EFFECTS__DELAY_HPP
#define DEVICES__EFFECTS__DELAY_HPP

#include <algorithm>
#include "../effect_device.hpp"

namespace Devices {
	namespace Effects {
		class Delay : public EffectDevice {
			public:
				static constexpr size_t BUFFER_SIZE = 1024 * 1024 * 4;

				Delay(int timeMs = 250, int decay = 64, int mix = 0)
				: EffectDevice("Delay", {
					Parameter("timeMs",    0, 10000, timeMs, _timeMs),
					Parameter("decay",     0,   128, decay,  _decay),
					Parameter("mix",    -127,   127, mix,    _mix)
				  })
				{}

				StereoSample apply(const Timer &, const StereoSample &);

			private:
				StereoSample _buffer[BUFFER_SIZE];
				int _timeMs;
				int _decay;
				int _mix;

				float timeSeconds() const {
					return _timeMs / 1000.f;
				}

				float decay() const {
					return _decay / 100.f;
				}

				float mix() const {
					return _mix / 127.f;
				}
		};
	}
}

#endif
