#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <map>
#include <list>
#include "base_device.hpp"
#include "../units/oscillator.hpp"
#include "../units/adsr.hpp"
#include "../stereo_sample.hpp"

namespace Devices {
class Synth : public BaseDevice {
	public:
		struct Voice {
			Voice(Oscillator::Type oscillatorType, int note, float velocity, float noteOnTime = 0.0)
			: note(note),
			  velocity(velocity),
			  oscillator(oscillatorType),
			  noteOnTime(noteOnTime),
			  noteOffTime(-1.0)
			{}

			Voice(int note, float velocity, float noteOnTime = 0.0)
			: note(note),
			  velocity(velocity),
			  noteOnTime(noteOnTime),
			  noteOffTime(-1.0)
			{}

			int note;
			float velocity;
			Oscillator oscillator;
			float noteOnTime;
			float noteOffTime;
		};

		Synth(Oscillator::Type oscillatorType)
		: BaseDevice("Synth"),
		  pitchBendRange(2.0),
		  transpose(0),
		  _oscillatorType(oscillatorType),
		  _panning(0.0)
		{}

		void setPanning(float value) {
			_panning = value;
		}

		void noteOn(const Timer &timer, int note, float velocity = 2.0) {
			_voices.push_back(Voice(_oscillatorType, note, velocity, timer.seconds()));
		}

		void noteOff(const Timer &timer, int note) {
			for (auto &v : _voices) {
				if (v.note == note && v.noteOffTime < 0.0) {
					v.noteOffTime = timer.seconds();
				}
			}
		}

		void update(const Timer &timer, float pitchBend = 0.0) {
			removeFinishedVoices(timer);

			float v = 0.0;

			for (auto &voice : _voices) {
				float freq = Utils::noteToFrequency(voice.note + transpose + pitchBendRange * pitchBend);
				float value = voice.oscillator.update(freq, timer);
				float env = envelope.update(timer, voice.noteOnTime, voice.noteOffTime);

				v += Utils::volume(value * voice.velocity * env * amplitude);
			}

			StereoSample out = Utils::pan(v, _panning);
			output(timer, out);
		}

		ADSR envelope;
		float amplitude;
		float pitchBendRange;
		int transpose;

	private:
		Oscillator::Type _oscillatorType;
		std::vector<Voice> _voices;
		float _panning;

		void removeFinishedVoices(const Timer &timer) {
			for (auto it = _voices.begin(); it != _voices.end();) {
				auto const &voice = *it;

				if (envelope.isNoteDone(timer, voice.noteOffTime)) {
					it = _voices.erase(it);
				} else {
					++it;
				}
			}
		}
};
};

#endif
