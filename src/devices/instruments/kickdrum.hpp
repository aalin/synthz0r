#ifndef DEVICES__INSTRUMENTS__KICKDRUM_HPP
#define DEVICES__INSTRUMENTS__KICKDRUM_HPP

#include "../instrument_device.hpp"
#include "../../units/oscillator.hpp"
#include "../../units/adsr.hpp"
#include "../../units/state_variable_filter.hpp"
#include "../../stereo_sample.hpp"
#include "../../note_event.hpp"

namespace Devices {
	namespace Instruments {
		class Kickdrum : public InstrumentDevice {
			public:
				Kickdrum();

				StereoSample apply(const Transport &transport, const NoteEventList &events);

			private:
				Units::Oscillator _oscillator;
				Units::ADSR _envelope;
				Units::ADSR _pitchEnvelope;
				Units::StateVariableFilter _filter;
				int _panning;
				int _amplitude;
				int _pitch;
				float _noteOnTime;

				float amplitude() {
					return _amplitude / 100.f;
				}

				float panning() {
					return _panning / 127.f;
				}

				void handleEvents(const Transport &transport, const NoteEventList &events) {
					for (const auto &event : events) {
						switch (event.type) {
							case NoteEvent::Type::NOTE_ON:
								noteOn(transport, event.note, event.velocity / 100.0);
								break;
							default:
								break;
						}
					}
				}

				void noteOn(const Transport &transport, int, float) {
					_noteOnTime = transport.secondsElapsedSinceStart();
				}
		};
	}
}

#endif
