#ifndef DEVICES__KICKDRUM_HPP
#define DEVICES__KICKDRUM_HPP

#include "base_device.hpp"
#include "../units/oscillator.hpp"
#include "../units/adsr.hpp"
#include "../units/state_variable_filter.hpp"
#include "../stereo_sample.hpp"
#include "../note_event.hpp"

namespace Devices {
	class Kickdrum : public BaseDevice {
		public:
			Kickdrum();

			void input(const Timer &timer, const NoteEvent &event) {
				switch (event.type) {
					case NoteEvent::Type::NOTE_ON:
						std::cout << name() << " received a NoteOn_" << (int)event.note << std::endl;
						noteOn(timer, event.note, event.velocity / 100.0);
						break;
					default:
						break;
				}
			}

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
