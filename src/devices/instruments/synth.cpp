#include "synth.hpp"

namespace Devices::Instruments {
	StereoSample Synth::apply(const Timer &timer, const NoteEventList &events) {
		handleEvents(timer, events);

		float pitchBend = 0.0;
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

		return Utils::pan(
			v * amplitude() / 10.0,
			panning()
		);
	}
}