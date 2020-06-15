#include "synth.hpp"

void Devices::Synth::update(const Timer &timer, float pitchBend) {
	float v = 0.0;

	for (auto it = _voices.begin(); it != _voices.end();) {
		auto &voice = *it;

		if (_envelope.isNoteDone(timer, voice.noteOffTime)) {
			it = _voices.erase(it);
			continue;
		} else {
			++it;
		}

		float freq = Utils::noteToFrequency(voice.note + _transpose + _pitchBendRange * pitchBend);
		float value = voice.oscillator.update(freq, timer);
		float env = _envelope.update(timer, voice.noteOnTime, voice.noteOffTime);

		v += value * voice.velocity * env;
	}

	if (_filterEnabled) {
		v = _filter.update(timer, v);
	}

	StereoSample out = Utils::pan(
		v * amplitude() / 10.0,
		panning()
	);

	output(timer, out);
}
