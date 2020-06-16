#include "kickdrum.hpp"

Devices::Kickdrum::Kickdrum()
: BaseDevice("Kickdrum", {
	Parameter("panning",            -127,   127,    0, _panning),
	Parameter("amplitude",             0,   128,  100, _amplitude),
	Parameter("envelope.attackMs",     0,  1000,   50, _envelope._attackMs),
	Parameter("envelope.decayMs",      0,  1000,   80, _envelope._decayMs),
	Parameter("envelope.sustain",      0,   127,    0, _envelope._sustain),
	Parameter("envelope.releaseMs",    0,  1000,    0, _envelope._releaseMs),
	Parameter("filter.cutoffHz",       0, 10000,  500, _filter._cutoffHz),
	Parameter("filter.resonance",      0,  1000,  200, _filter._resonance),
	Parameter("filter.bandwidth",      0,  1000,  900, _filter._bandwidth),
  }),
  _oscillator(Units::Oscillator::Type::SINE),
  _noteOnTime(-1.0)
{
	_filter._type = Units::StateVariableFilter::Type::BANDPASS;
	_pitchEnvelope._attackMs = 0;
	_pitchEnvelope._decayMs = 100;
	_pitchEnvelope._sustain = 0;
	_pitchEnvelope._releaseMs = 0;
}

void Devices::Kickdrum::update(const Timer &timer, float) {
	if (_noteOnTime < 0.0) {
		return;
	}

	const float dt = timer.seconds() - _noteOnTime;

	if (dt < 0.0) {
		return;
	}

	float speed = 0.13;

	if (dt > speed) {
		return;
	}

	float freq = 330.0 * (1.0 - dt / speed);

	float v = _oscillator.update(freq, timer) * _envelope.update(timer, _noteOnTime, -1.0);
	v = _filter.update(timer, v);

	StereoSample out = Utils::pan(
		v * amplitude() / 10.0,
		panning()
	);

	output(timer, out);
}