#ifndef WAVETABLE_SYNTH_HPP
#define WAVETABLE_SYNTH_HPP

#include "base_device.hpp"
#include "../units/adsr.hpp"
#include "../units/phase.hpp"
#include "../waveform.hpp"
#include "../utils.hpp"

namespace Devices {
class WavetableSynth : public BaseDevice {
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

			float update(float freq, const Timer &timer) {
				return waveform.getValue(
					_phase.update(freq, timer.sampleRate())
				);
			}

			private:
			Units::Phase _phase;
		};

		WavetableSynth()
		: BaseDevice("Wavetable Synth", {
			Parameter("pitchBendRange",        0,                         24,   2, _pitchBendRange),
			Parameter("transpose",           -24,                        100,   2, _transpose),
			Parameter("panning",            -127,                        127,   0, _panning),
			Parameter("amplitude",             0,                        128,   0, _amplitude),
			Parameter("envelope.attackMs",     0,                       1000, 150, _envelope._attackMs),
			Parameter("envelope.decayMs",      0,                       1000,   0, _envelope._decayMs),
			Parameter("envelope.sustain",      0,                        127,   0, _envelope._sustain),
			Parameter("envelope.releaseMs",    0,                       1000,   0, _envelope._releaseMs),
			Parameter("waveformIndex",         0, Waveform::WAVEFORMS.size()-1, 0, _waveformIndex)
		  })
		{}

		void input(const Timer &timer, const NoteEvent &event) {
			switch (event.type) {
				case NoteEvent::Type::NOTE_ON:
					noteOn(timer, event.note, event.velocity / 255.0);
					break;
				case NoteEvent::Type::NOTE_OFF:
					noteOff(timer, event.note);
					break;
				default:
					break;
			}
		}

		void noteOn(const Timer &timer, int note, float velocity = 1.0) {
			_voices.push_back(Voice(_waveformIndex, note, velocity, timer.seconds()));
		}

		void noteOff(const Timer &timer, int note) {
			for (auto &v : _voices) {
				if (v.note == note && v.noteOffTime < 0.0) {
					v.noteOffTime = timer.seconds();
				}
			}
		}

		const std::string &getWaveformName() {
			return Waveform::WAVEFORMS.at(_waveformIndex).name;
		}

		void update(const Timer &timer, float pitchBend = 0.0);

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
};
}

#endif
