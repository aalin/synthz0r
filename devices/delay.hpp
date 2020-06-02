#ifndef DEVICES__DELAY_HPP
#define DEVICES__DELAY_HPP

#include <algorithm>
#include "base_device.hpp"

namespace Devices {
class Delay : public BaseDevice {
	public:
		static constexpr size_t BUFFER_SIZE = 1024 * 1024 * 4;

		Delay(int time = 250, int decay = 64, int mix = 0)
		: BaseDevice(
			"Delay", {
				Variable("time", 0, 10000, time),
				Variable("decay", 0, 128, decay),
				Variable("mix", -127, 127, mix)
			}
		  )
		{
			for (size_t i = 0; i < BUFFER_SIZE; i++) {
				_buffer[i] = 0.0;
			}
		}

		void input(const Timer &timer, const float &value) {
			output(timer, apply(timer, value));
		}

		void input(const Timer &timer, const StereoSample &sample) {
			output(timer, StereoSample(apply(timer, sample.mono())));
		}

	private:
		float _buffer[BUFFER_SIZE];

		float time() const {
			return getValue("time") / 1000.0;
		}

		float mix() const {
			return getValue("mix") / 127.f;
		}

		float decay() const { return getValue("decay") / 100.0; }

		float apply(const Timer &timer, const float &value) {
			const size_t t = timer.tick() % BUFFER_SIZE;

			const size_t delaySamples = static_cast<size_t>(time() * timer.sampleRate());
			const size_t index = Utils::mod(t + delaySamples, BUFFER_SIZE);

			const float v = Utils::lerp(value, _buffer[t], mix());

			_buffer[index] = v * decay();

			return v;
		}
};
};

#endif
