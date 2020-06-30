#include "snare.hpp"

namespace Devices::Instruments::Drums {
	Snare::Voice::Voice(const Units::ADSR2::Settings &noiseEnvSettings, const Units::ADSR2::Settings &toneEnvSettings, float pitch)
	: _noiseEnv(noiseEnvSettings),
	  _toneEnv(toneEnvSettings),
	  _noiseOsc(Units::Oscillator::Type::NOISE),
	  _toneOsc(Units::Oscillator::Type::TRIANGLE),
	  _pitch(pitch)
	{
		_noiseFilter._type = Units::StateVariableFilter::Type::HIGHPASS;
		_noiseFilter._cutoffHz = 10000;
	}

	float Snare::Voice::update(const Transport &transport, const Voices::VoiceData &voiceData, const float &) {
		const Units::ADSR2::Settings noiseEnvSettings = _noiseEnv.settings();
		const float noteOnTime = voiceData.noteOnTime();
		const float noteOffTime = noteOnTime + noiseEnvSettings.attack() + noiseEnvSettings.decay();

		const Timer &timer = transport.timer();

		float v = 0.0;

		v += _noiseOsc.update(200, timer) * _noiseEnv.update(timer, noteOnTime, noteOffTime);

		v = _noiseFilter.update(timer, v);

		v += _toneOsc.update(200, timer) * _toneEnv.update(timer, noteOnTime, noteOffTime);

		v *= voiceData.velocity();

		return v;
	}

	Snare::Snare() {
		noiseEnvSettings._attackMs = 10;
		noiseEnvSettings._decayMs = 100;
		noiseEnvSettings._sustain = 0;
		noiseEnvSettings._releaseMs = 0;

		toneEnvSettings._attackMs = 10;
		toneEnvSettings._decayMs = 50;
		toneEnvSettings._sustain = 0;
		toneEnvSettings._releaseMs = 0;

		pitch = 40;
	}
}


