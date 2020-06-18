#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <list>
#include "base_device.hpp"
#include "../units/oscillator.hpp"
#include "../units/state_variable_filter.hpp"
#include "../units/adsr.hpp"
#include "../stereo_sample.hpp"

namespace Devices {
class Synth : public BaseDevice {
	public:
		struct Voice {
			Voice(int oscillatorType, int note, float velocity, float noteOnTime = 0.0)
			: note(note),
			  velocity(velocity),
			  noteOnTime(noteOnTime),
			  noteOffTime(-1.0),
			  oscillator(oscillatorType)
			{}

			int note;
			float velocity;
			float noteOnTime;
			float noteOffTime;
			Units::Oscillator oscillator;
		};

		Synth()
		: BaseDevice("Synth", {
			Parameter("pitchBendRange",        0,    24,    2, _pitchBendRange),
			Parameter("transpose",           -24,    24,    2, _transpose),
			Parameter("oscillatorType",        0,     5,    0, _oscillatorType),
			Parameter("panning",            -127,   127,    0, _panning),
			Parameter("amplitude",             0,   128,    0, _amplitude),
			Parameter("envelope.attackMs",     0,  1000,  150, _envelope._attackMs),
			Parameter("envelope.decayMs",      0,  1000,    0, _envelope._decayMs),
			Parameter("envelope.sustain",      0,   127,    0, _envelope._sustain),
			Parameter("envelope.releaseMs",    0,  1000,    0, _envelope._releaseMs),
			Parameter("filter.enabled",        0,     1,    1, _filterEnabled),
			Parameter("filter.cutoffHz",       0, 10000, 8000, _filter._cutoffHz),
			Parameter("filter.resonance",      0,  1000,  200, _filter._resonance),
			Parameter("filter.bandwidth",      0,  1000,  500, _filter._bandwidth),
			Parameter("filter.type",           0,     4,    0, _filter._typeInt),
		  })
		{}

		void input(const Timer &timer, const NoteEvent &event) {
			switch (event.type) {
				case NoteEvent::Type::NOTE_ON:
					std::cout << name() << " received a NoteOn_" << (int)event.note << std::endl;
					noteOn(timer, event.note, event.velocity / 100.0);
					break;
				case NoteEvent::Type::NOTE_OFF:
					std::cout << name() << " received a NoteOff_" << (int)event.note << std::endl;
					noteOff(timer, event.note);
					break;
				default:
					break;
			}
		}

		void noteOn(const Timer &timer, int note, float velocity = 1.0) {
			_voices.push_back(Voice(_oscillatorType, note, velocity, timer.seconds()));
		}

		void noteOff(const Timer &timer, int note) {
			for (auto &v : _voices) {
				if (v.note == note && v.noteOffTime < 0.0) {
					v.noteOffTime = timer.seconds();
				}
			}
		}

		void update(const Timer &timer, float pitchBend = 0.0);

	private:
		Units::ADSR _envelope;
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

		std::list<Voice> _voices;
};
}

#endif
