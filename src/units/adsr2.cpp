#include "adsr2.hpp"

float Units::ADSR2::update(const Timer &timer, float noteOnTime, float noteOffTime) {
	const float time = timer.seconds();

	switch (_state) {
		case State::ATTACKING: {
			const float dt = std::max(0.f, time - noteOnTime);

			if (dt >= _settings.attack()) {
				_state = State::DECAYING;
				return 1.0f;
			}

			return Utils::cosineInterpolate(_settings.attackStart(), 1.0f, dt / _settings.attack());
		}
		case State::DECAYING: {
			const float dt = std::max(0.f, time - noteOnTime);

			if (dt >= _settings.attack() + _settings.decay()) {
				_state = State::SUSTAINING;
				return _settings.sustain();
			}

			return Utils::cosineInterpolate(1.0f, _settings.sustain(), (dt - _settings.attack()) / _settings.decay());
		}
		case State::SUSTAINING: {
			if (noteOffTime >= 0.0 && time > noteOffTime) {
				_state = State::RELEASING;
			}

			return _settings.sustain();
		}
		case State::RELEASING: {
			const float dt = std::max(0.f, time - noteOffTime);

			if (dt >= _settings.release()) {
				_state = State::OFF;
				return 0.0f;
			}

			return Utils::cosineInterpolate(_settings.sustain(), 0.0f, dt / _settings.release());
		}
		default:
			return 0.0f;
	}
}
