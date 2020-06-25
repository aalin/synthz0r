#ifndef DEVICES__INSTRUMENTS__WAVETABLE_SYNTH_HPP
#define DEVICES__INSTRUMENTS__WAVETABLE_SYNTH_HPP

#include "../instrument_device.hpp"
#include "../../units/adsr.hpp"
#include "../../units/phase.hpp"
#include "../../waveform.hpp"
#include "../../utils.hpp"

namespace Devices::Instruments {
	class WavetableSynth : public InstrumentDevice {
		public:
			struct Voice {
				Voice(size_t waveformIndex, int note, float velocity, float noteOnTime = 0.0)
				: waveform(Waveform::WAVEFORMS.at(waveformIndex)),
				  note(note),
				  velocity(velocity),
				  noteOnTime(noteOnTime),
				  noteOffTime(-1)
				{}

				const Waveform &waveform;
				int note;
				float velocity;
				float noteOnTime;
				float noteOffTime;

				float update(float freq, const Transport &transport) {
					return waveform.getValue(
						_phase.update(freq, transport.sampleRate())
					);
				}

				private:
				Units::Phase _phase;
			};

			WavetableSynth()
			: InstrumentDevice("Wavetable Synth") {
				setupParameters({
					Parameter("pitchBendRange",        0,                         24,   2, _pitchBendRange),
					Parameter("transpose",           -24,                        100,   0, _transpose),
					Parameter("panning",            -127,                        127,   0, _panning),
					Parameter("amplitude",             0,                        128, 100, _amplitude),
					Parameter("envelope.attackMs",     0,                       1000, 150, _envelope._attackMs),
					Parameter("envelope.decayMs",      0,                       1000,  60, _envelope._decayMs),
					Parameter("envelope.sustain",      0,                        127,  30, _envelope._sustain),
					Parameter("envelope.releaseMs",    0,                       1000,  70, _envelope._releaseMs),
					Parameter("waveformIndex",         0, Waveform::WAVEFORMS.size()-1, 0, _waveformIndex)
				});
			}

			const std::string &getWaveformName() {
				return Waveform::WAVEFORMS.at(_waveformIndex).name;
			}

			StereoSample apply(const Transport &transport, const NoteEventList &events);

		private:
			Units::ADSR _envelope;
			int _amplitude;
			int _pitchBendRange;
			int _transpose;
			int _panning;
			int _waveformIndex;

			std::list<Voice> _voices;

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
							pauseAll(transport);
							return;
						case NoteEvent::Type::NOTE_ON:
							noteOn(transport, event.note, event.velocity / 255.0);
							break;
						case NoteEvent::Type::NOTE_OFF:
							noteOff(transport, event.note);
							break;
						default:
							break;
					}
				}
			}

			void noteOn(const Transport &transport, int note, float velocity = 1.0) {
				_voices.push_back(Voice(_waveformIndex, note, velocity, transport.secondsElapsedSinceStart()));
			}

			void noteOff(const Transport &transport, int note) {
				for (auto &v : _voices) {
					if (v.note == note && v.noteOffTime < 0.0) {
						v.noteOffTime = transport.secondsElapsedSinceStart();
					}
				}
			}

			void pauseAll(const Transport &transport) {
				for (auto &v : _voices) {
					v.noteOffTime = transport.secondsElapsedSinceStart();
				}
			}
	};
}

#endif
