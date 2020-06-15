#ifndef DEVICES__KICKDRUM_HPP
#define DEVICES__KICKDRUM_HPP

#include "base_device.hpp"
#include "../units/oscillator.hpp"
#include "../units/adsr.hpp"
#include "../units/state_variable_filter.hpp"
#include "../stereo_sample.hpp"

namespace Devices {
	class Kickdrum : public BaseDevice {
		public:
			Kickdrum();

			void noteOn(const Timer &timer, int, float) {
				std::cout << "Trigger kick" << std::endl;
				_noteOnTime = timer.seconds();
			}

			void update(const Timer &timer, float);

		private:
			Units::Oscillator _oscillator;
			Units::ADSR _envelope;
			Units::ADSR _pitchEnvelope;
			Units::StateVariableFilter _filter;
			int _panning;
			int _amplitude;
			float _noteOnTime;

			float amplitude() {
				return _amplitude / 100.f;
			}

			float panning() {
				return _panning / 127.f;
			}
	};
}

#endif
