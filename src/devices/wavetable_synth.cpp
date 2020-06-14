#include "wavetable_synth.hpp"

void Devices::WavetableSynth::update(const Timer &timer, float pitchBend) {
	float v = 0.0;

	for (auto it = _voices.begin(); it != _voices.end();) {
		auto &voice = *it;

		if (_envelope.isNoteDone(timer, voice.noteOffTime)) {
			std::cout << "Removing voice" << std::endl;
			it = _voices.erase(it);
			continue;
		} else {
			++it;
		}

		constexpr int waveTranspose = 3;
		float freq = Utils::noteToFrequency(voice.note + waveTranspose + _transpose + _pitchBendRange * pitchBend);
		float value = voice.update(freq, timer);
		float env = _envelope.update(timer, voice.noteOnTime, voice.noteOffTime);

		v += Utils::volume(value * voice.velocity * env * amplitude());
	}

	StereoSample out = Utils::pan(v, panning());

	output(timer, out);
}
