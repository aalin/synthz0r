#include "synth.hpp"

namespace Devices::Instruments {
	float Synth::Voice::update(const Transport &transport, const Voices::VoiceData &voiceData, const float &transpose) {
		float freq = ::Utils::noteToFrequency(voiceData.note() + transpose);

		float env = _envelope.update(
			transport.timer(),
			voiceData.noteOnTime(),
			voiceData.noteOffTime()
		);

		float value = _oscillator.update(freq, transport.timer());

		return value * voiceData.velocity() * env;
	}

	StereoSample Synth::apply(const Transport &transport, const NoteEventList &events) {
		handleEvents(transport, events);

		const Timer &timer = transport.timer();

		float pitchBend = 0.0;

		float transpose = _transpose + _pitchBendRange * pitchBend;

		float v = _voices.update(transport, transpose);

		if (_filterEnabled) {
			v = _filter.update(timer, v);
		}

		return Utils::pan(
			v * amplitude() * 0.2,
			panning()
		);
	}
}
