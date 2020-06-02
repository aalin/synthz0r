#ifndef DEVICES__DELAY_HPP
#define DEVICES__DELAY_HPP

#include <algorithm>
#include "base_device.hpp"

namespace Devices {
class Delay : public BaseDevice {
	public:
		static constexpr size_t BUFFER_SIZE = 1024 * 1024 * 4;

		Delay(float time, float decay, float mix)
		: BaseDevice("Delay"),
		  _time(time),
		  _decay(decay),
		  _mix(mix)
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
		float _time;
		float _decay;
		float _mix;
		float _buffer[BUFFER_SIZE];

		float apply(const Timer &timer, const float &value) {
			const size_t t = timer.tick() % BUFFER_SIZE;
			size_t delaySamples = static_cast<size_t>(_time / 1000.0 * timer.sampleRate());
			size_t index = Utils::mod(t + delaySamples, BUFFER_SIZE);

			float v = Utils::lerp(value, _buffer[t], _mix);
			_buffer[index] = v * _decay;
			return v;
		}
};
};

#endif
