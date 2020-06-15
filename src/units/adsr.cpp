#include "adsr.hpp"

float Units::ADSR::update(const Timer &timer, float noteOnTime, float noteOffTime) {
	const float time = timer.seconds();

	if (noteOffTime >= 0.0 && time >= noteOffTime) {
		const float dt = std::max(0.f, time - noteOffTime);

		if (dt > release()) {
			return 0.0;
		}

		return Utils::lerp(sustain(), 0.0f, dt / release());
	}

	const float dt = std::max(0.f, time - noteOnTime);

	if (dt < attack()) {
		return Utils::lerp(0.0f, 1.0f, dt / attack());
	}

	if (dt < attack() + decay()) {
		return Utils::lerp(1.0f, sustain(), (dt - attack()) / decay());
	}

	return sustain();
}
