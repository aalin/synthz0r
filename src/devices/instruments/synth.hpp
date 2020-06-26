#ifndef DEVICES__INSTRUMENTS__SYNTH_HPP
#define DEVICES__INSTRUMENTS__SYNTH_HPP

#include "../instrument_device.hpp"
#include "../../units/oscillator.hpp"
#include "../../units/state_variable_filter.hpp"
#include "../../units/adsr2.hpp"
#include "../../stereo_sample.hpp"
#include "voices/voice_list.hpp"

namespace Devices::Instruments {
	class Synth : public InstrumentDevice {
		public:
			struct Voice : public Voices::AbstractVoice {
				public:
					Voice(int oscillatorType, Units::ADSR2::Settings envelopeSettings)
					: _oscillator(oscillatorType),
					  _envelope(envelopeSettings)
					{}

					float update(const Transport &transport, const Voices::VoiceData &voiceData, const float &transpose);

					bool hasStopped() {
						return _envelope.isOff();
					}

				private:
					Units::Oscillator _oscillator;
					Units::ADSR2 _envelope;
			};

			Synth()
			: InstrumentDevice("Synth") {
				setupParameters({
					Parameter("pitchBendRange",        0,    24,    2, _pitchBendRange),
					Parameter("transpose",           -24,    24,    0, _transpose),
					Parameter("oscillatorType",        0,     5,    0, _oscillatorType),
					Parameter("panning",            -127,   127,    0, _panning),
					Parameter("amplitude",             0,   128,  100, _amplitude),
					Parameter("envelope.attackStart",  0,   100,    0, _envelopeSettings._attackStart),
					Parameter("envelope.attackMs",     0,  1000,  150, _envelopeSettings._attackMs),
					Parameter("envelope.decayMs",      0,  1000,    0, _envelopeSettings._decayMs),
					Parameter("envelope.sustain",      0,   100,    0, _envelopeSettings._sustain),
					Parameter("envelope.releaseMs",    0,  1000,    0, _envelopeSettings._releaseMs),
					Parameter("filter.enabled",        0,     1,    1, _filterEnabled),
					Parameter("filter.cutoffHz",       0, 10000, 8000, _filter._cutoffHz),
					Parameter("filter.resonance",      0,  1000,  200, _filter._resonance),
					Parameter("filter.bandwidth",      0,  1000,  500, _filter._bandwidth),
					Parameter("filter.type",           0,     4,    0, _filter._typeInt),
				});
			}

			StereoSample apply(const Transport &transport, const NoteEventList &events);

		private:
			Units::ADSR2::Settings _envelopeSettings;
			Units::StateVariableFilter _filter;

			int _oscillatorType;
			int _amplitude;
			int _pitchBendRange;
			int _transpose;
			int _panning;
			int _filterEnabled;

			float amplitude() const {
				return _amplitude / 100.0;
			}

			float panning() const {
				return _panning / 127.0;
			}

			Voices::VoiceList _voices;

			void handleEvents(const Transport &transport, const NoteEventList &events) {
				for (const auto &event : events) {
					switch (event.type) {
						case NoteEvent::Type::PAUSE_ALL:
							_voices.stopAll(transport);
							return;
						case NoteEvent::Type::NOTE_ON:
							_voices.play(
								transport,
								event.note,
								event.velocity / 128.f,
								std::make_unique<Voice>(
									_oscillatorType,
									_envelopeSettings
								)
							);
							break;
						case NoteEvent::Type::NOTE_OFF:
							_voices.stop(transport, event.note);
							break;
						default:
							break;
					}
				}
			}
	};
}

#endif
