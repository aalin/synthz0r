#include "hihat.hpp"

const static float PITCHES[] = {
	2.0,
	3.0,
	4.16,
	5.43,
	6.79,
	8.21
};

namespace Devices::Instruments::Drums {
	Hihat::Voice::Voice(const Units::ADSR2::Settings &volumeEnvelopeSettings, float pitch)
	: _volumeEnvelope(volumeEnvelopeSettings),
	  _pitch(pitch)
	{
		for (int i = 0; i < 6; i++) {
			_oscillators.push_back(Units::Oscillator(Units::Oscillator::Type::SQUARE));
		}

		_bandpass._type = Units::StateVariableFilter::Type::BANDPASS;
		_bandpass._cutoffHz = 100000;
		_highpass._type = Units::StateVariableFilter::Type::HIGHPASS;
		_highpass._cutoffHz = 70000;
	}

	float Hihat::Voice::update(const Transport &transport, const Voices::VoiceData &voiceData, const float &) {
		const Units::ADSR2::Settings volumeEnvelopeSettings = _volumeEnvelope.settings();
		const float noteOnTime = voiceData.noteOnTime();
		const float noteOffTime = noteOnTime + volumeEnvelopeSettings.attack() + volumeEnvelopeSettings.decay();

		const Timer &timer = transport.timer();

		float v = 0.0;

		for (size_t i = 0; i < 6; i++) {
			v += _oscillators[i].update(_pitch * PITCHES[i], timer);
		}

		v = _bandpass.update(timer, v);
		v = _highpass.update(timer, v);

		v *= _volumeEnvelope.update(timer, noteOnTime, noteOffTime);

		v *= voiceData.velocity();

		return v;
	}

	Hihat::Hihat() {
		volumeEnvelopeSettings._attackMs = 10;
		volumeEnvelopeSettings._decayMs = 100;
		volumeEnvelopeSettings._sustain = 0;
		volumeEnvelopeSettings._releaseMs = 0;

		pitch = 40;
	}
}


