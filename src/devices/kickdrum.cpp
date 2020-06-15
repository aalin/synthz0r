#include "kickdrum.hpp"

Devices::Kickdrum::Kickdrum()
: BaseDevice("Kickdrum", {
	Parameter("panning",            -127,   127,    0, _panning),
	Parameter("amplitude",             0,   128,  100, _amplitude),
	Parameter("envelope.attackMs",     0,  1000,   50, _envelope._attackMs),
	Parameter("envelope.decayMs",      0,  1000,   80, _envelope._decayMs),
	Parameter("envelope.sustain",      0,   127,    0, _envelope._sustain),
	Parameter("envelope.releaseMs",    0,  1000,    0, _envelope._releaseMs),
  }),
  _oscillator(Units::Oscillator::Type::SINE),
  _noteOnTime(-1.0)
{
	_freqEnvelope._attackMs = 0;
	_freqEnvelope._decayMs = 100;
	_freqEnvelope._sustain = 0;
	_freqEnvelope._releaseMs = 0;
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

	StereoSample out = Utils::pan(
		v * amplitude(),
		panning()
	);

	output(timer, out);
}
