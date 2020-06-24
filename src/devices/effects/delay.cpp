#include "delay.hpp"

namespace Devices::Effects {
	StereoSample Delay::apply(const Transport &transport, const StereoSample &sample) {
		const size_t t = transport.timer().ticks() % BUFFER_SIZE;

		const size_t delaySamples = static_cast<size_t>(timeSeconds() * transport.sampleRate());
		const size_t index = Utils::mod(t + delaySamples, BUFFER_SIZE);

		StereoSample out(
			Utils::lerp(sample.l, _buffer[t].l, mix()),
			Utils::lerp(sample.r, _buffer[t].r, mix())
		);

		_buffer[index].l = out.l * decay();
		_buffer[index].r = out.r * decay();

		return out;
	}
}
