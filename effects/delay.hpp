#ifndef DELAY_HPP
#define DELAY_HPP

#include <algorithm>

namespace Effects {
class Delay : public BaseEffect {
	public:
		static constexpr size_t bufferSize = 1024 * 1024 * 4;

		Delay(float time, float decay, float mix, float sampleRate)
		: _time(time),
		  _decay(decay),
		  _mix(mix),
		  _sampleRate(sampleRate),
		  _tick(0)
		{
			for (size_t i = 0; i < bufferSize; i++) {
				_buffer[i] = 0.0;
			}
		}

		float apply(float value) {
			const size_t t = tick();
			size_t delaySamples = static_cast<size_t>(_time / 1000.0 * _sampleRate);
			size_t index = Utils::mod(t + delaySamples, bufferSize);

			float v = Utils::lerp(value, _buffer[t], _mix);
			_buffer[index] = v * _decay;
			return v;
		}

	private:
		float _time;
		float _decay;
		float _mix;
		float _sampleRate;
		size_t _tick;
		float _buffer[bufferSize];

		size_t tick() {
			const size_t t = _tick++;

			if (_tick > bufferSize) {
				_tick = 0;
			}

			return t;
		}
};
}


#endif
