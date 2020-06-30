#include "kick.hpp"

namespace Devices::Instruments::Drums {
	Kick::Voice::Voice(const Units::ADSR2::Settings &pitchEnvelopeSettings, const Units::ADSR2::Settings &volumeEnvelopeSettings, float pitch)
	: _pitchEnvelope(pitchEnvelopeSettings),
	  _volumeEnvelope(volumeEnvelopeSettings),
	  _oscillator(Units::Oscillator::Type::SINE),
	  _pitch(pitch)
	{}

	float Kick::Voice::update(const Transport &transport, const Voices::VoiceData &voiceData, const float &) {
		const Units::ADSR2::Settings volumeEnvelopeSettings = _volumeEnvelope.settings();
		const float noteOnTime = voiceData.noteOnTime();
		const float noteOffTime = noteOnTime + volumeEnvelopeSettings.attack() + volumeEnvelopeSettings.decay();

		const Timer &timer = transport.timer();

		float freq = _pitch * _pitchEnvelope.update(timer, noteOnTime, -1.0f);

		return _oscillator.update(freq, timer) * _volumeEnvelope.update(timer, noteOnTime, noteOffTime);
	}

	Kick::Kick() {
		pitchEnvelopeSettings._attackStart = 100;
		pitchEnvelopeSettings._attackMs = 0;
		pitchEnvelopeSettings._decayMs = 100;
		pitchEnvelopeSettings._sustain = 0;
		pitchEnvelopeSettings._releaseMs = 0;

		volumeEnvelopeSettings._attackMs = 20;
		volumeEnvelopeSettings._decayMs = 100;
		volumeEnvelopeSettings._sustain = 0;
		volumeEnvelopeSettings._releaseMs = 0;
	}
}
