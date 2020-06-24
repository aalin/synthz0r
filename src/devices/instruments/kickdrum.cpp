#include "kickdrum.hpp"

namespace Devices::Instruments {
	Kickdrum::Kickdrum()
	: InstrumentDevice("Kickdrum"),
	  _oscillator(Units::Oscillator::Type::SINE),
	  _noteOnTime(-1.0)
	{
		setupParameters({
			Parameter("panning",            -127,   127,    0, _panning),
			Parameter("amplitude",             0,   128,  100, _amplitude),
			Parameter("pitch",                 1,  1760,  440, _pitch),
			Parameter("envelope.attackMs",     0,  1000,   50, _envelope._attackMs),
			Parameter("envelope.decayMs",      0,  1000,   80, _envelope._decayMs),
			Parameter("envelope.sustain",      0,   127,    0, _envelope._sustain),
			Parameter("envelope.releaseMs",    0,  1000,    0, _envelope._releaseMs),
			Parameter("filter.cutoffHz",       0, 10000,  500, _filter._cutoffHz),
			Parameter("filter.resonance",      0,  1000,  200, _filter._resonance),
			Parameter("filter.bandwidth",      0,  1000,  900, _filter._bandwidth),
		});

		_filter._type = Units::StateVariableFilter::Type::BANDPASS;
		_pitchEnvelope._attackStart = 100;
		_pitchEnvelope._attackMs = 0;
		_pitchEnvelope._decayMs = 100;
		_pitchEnvelope._sustain = 0;
		_pitchEnvelope._releaseMs = 0;
	}

	StereoSample Kickdrum::apply(const Timer &timer, const NoteEventList &events) {
		handleEvents(timer, events);

		if (_noteOnTime < 0.0) {
			return StereoSample();
		}

		const float dt = timer.seconds() - _noteOnTime;

		if (dt < 0.0) {
			return StereoSample();
		}

		float freq = _pitch * _pitchEnvelope.update(timer, _noteOnTime, -1.0);

		float v = _oscillator.update(freq, timer) * _envelope.update(timer, _noteOnTime, -1.0);
		v = _filter.update(timer, v);

		return Utils::pan(
			v * amplitude(),
			panning()
		);
	}
}
