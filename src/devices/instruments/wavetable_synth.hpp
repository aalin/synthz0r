#ifndef DEVICES__INSTRUMENTS__WAVETABLE_SYNTH_HPP
#define DEVICES__INSTRUMENTS__WAVETABLE_SYNTH_HPP

#include "../instrument_device.hpp"
#include "../../units/adsr2.hpp"
#include "../../units/phase.hpp"
#include "../../waveform.hpp"
#include "../../utils.hpp"
#include "voices/voice_list.hpp"

namespace Devices::Instruments {
	class WavetableSynth : public InstrumentDevice {
		public:
			class Voice : public Voices::AbstractVoice {
				public:
					Voice(size_t waveformIndex, Units::ADSR2::Settings envelopeSettings);

					float update(const Transport &transport, const Voices::VoiceData &voiceData, const float &transpose);

					bool hasStopped() {
						return _envelope.isOff();
					}

				private:
					const Waveform &_waveform;
					Units::ADSR2 _envelope;
					Units::Phase _phase;
			};

			WavetableSynth() : InstrumentDevice("Wavetable Synth") {
				const int maxWf = Waveform::WAVEFORMS.size() - 1;

				setupParameters({
					Parameter("pitchBendRange",        0,    24,   2, _pitchBendRange),
					Parameter("transpose",           -24,    24,   0, _transpose),
					Parameter("panning",            -127,   127,   0, _panning),
					Parameter("amplitude",             0,   128, 100, _amplitude),
					Parameter("envelope.attackStart",  0,   100,   0, _envelopeSettings._attackStart),
					Parameter("envelope.attackMs",     0,  1000, 150, _envelopeSettings._attackMs),
					Parameter("envelope.decayMs",      0,  1000,  60, _envelopeSettings._decayMs),
					Parameter("envelope.sustain",      0,   100,  30, _envelopeSettings._sustain),
					Parameter("envelope.releaseMs",    0,  1000,  70, _envelopeSettings._releaseMs),
					Parameter("waveformIndex",         0, maxWf, 0, _waveformIndex)
				});
			}

			const std::string &getWaveformName() {
				return Waveform::WAVEFORMS.at(_waveformIndex).name;
			}

			StereoSample apply(const Transport &transport, const NoteEventList &events);

		private:
			Units::ADSR2::Settings _envelopeSettings;
			int _amplitude;
			int _pitchBendRange;
			int _transpose;
			int _panning;
			int _waveformIndex;

			Voices::VoiceList _voices;

			float amplitude() const {
				return _amplitude / 100.0;
			}

			float panning() const {
				return _panning / 127.0;
			}

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
									_waveformIndex,
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
