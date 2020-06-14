#ifndef DEVICES__DELAY_HPP
#define DEVICES__DELAY_HPP

#include <algorithm>
#include "base_device.hpp"

namespace Devices {
class Delay : public BaseDevice {
	public:
		static constexpr size_t BUFFER_SIZE = 1024 * 1024 * 4;

		Delay(int timeMs = 250, int decay = 64, int mix = 0)
		: BaseDevice("Delay", {
			Parameter("timeMs",    0, 10000, timeMs, _timeMs),
			Parameter("decay",     0,   128, decay,  _decay),
			Parameter("mix",    -127,   127, mix,    _mix)
		  })
		{}

		void input(const Timer &timer, const float &value) {
			output(timer, apply(timer, value));
		}

		void input(const Timer &timer, const StereoSample &sample) {
			output(timer, apply(timer, sample));
		}

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

		StereoSample apply(const Timer &timer, const StereoSample &sample) {
			const size_t t = timer.tick() % BUFFER_SIZE;

			const size_t delaySamples = static_cast<size_t>(timeSeconds() * timer.sampleRate());
			const size_t index = Utils::mod(t + delaySamples, BUFFER_SIZE);

			StereoSample out(
				Utils::lerp(sample.l, _buffer[t].l, mix()),
				Utils::lerp(sample.r, _buffer[t].r, mix())
			);

			_buffer[index].l = out.l * decay();
			_buffer[index].r = out.r * decay();

			return out;
		}
};
};

#endif
