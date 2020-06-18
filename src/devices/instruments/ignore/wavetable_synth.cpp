#include "wavetable_synth.hpp"

constexpr float sampleTranspose = 0.0f;

void Devices::WavetableSynth::update(const Timer &timer, float pitchBend) {
	float v = 0.0;

	float transpose = sampleTranspose + _transpose + _pitchBendRange * pitchBend;

	for (auto it = _voices.begin(); it != _voices.end();) {
		auto &voice = *it;

		if (_envelope.isNoteDone(timer, voice.noteOffTime)) {
			it = _voices.erase(it);
			continue;
		} else {
			++it;
		}

		float freq = Utils::noteToFrequency(voice.note + transpose);
		float value = voice.update(freq, timer);
		float env = _envelope.update(timer, voice.noteOnTime, voice.noteOffTime);

		v += value * voice.velocity * env;
	}

	StereoSample out = Utils::pan(
		v * amplitude() / 10.0,
		panning()
	);

	output(timer, out);
}
