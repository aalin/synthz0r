#include "wavetable_synth.hpp"

namespace Devices::Instruments {
	WavetableSynth::Voice::Voice(size_t waveformIndex, Units::ADSR2::Settings envelopeSettings)
	: _waveform(Waveform::WAVEFORMS.at(waveformIndex)),
	  _envelope(envelopeSettings)
	{}

	float WavetableSynth::Voice::update(const Transport &transport, const Voices::VoiceData &voiceData, const float &transpose) {
		float freq = ::Utils::noteToFrequency(voiceData.note() + transpose);

		float env = _envelope.update(
			transport.timer(),
			voiceData.noteOnTime(),
			voiceData.noteOffTime()
		);

		float value = _waveform.getValue(
			_phase.update(freq, transport.sampleRate())
		);

		return value * voiceData.velocity() * env;
	}

	StereoSample WavetableSynth::apply(const Transport &transport) {
		float pitchBend = 0.0;

		float transpose = _transpose + _pitchBendRange * pitchBend;

		float v = _voices.update(transport, transpose);

		return ::Utils::pan(
			v * amplitude() * 0.5,
			panning()
		);
	}
}
