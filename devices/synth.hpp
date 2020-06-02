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
		: BaseDevice("Synth", {
			Variable("pitchBendRange", 0, 24, 2, _pitchBendRange),
			Variable("transpose", -24, 24, 2, _transpose),
			Variable("oscillatorType", 0, 5, oscillatorType, reinterpret_cast<int&>(_oscillatorType)),
			Variable("panning", -127, 127, 0, _panning),
			Variable("amplitude", 0, 128, 0, _amplitude),
			Variable("envelope.attackMs", 0, 1000, 150, _envelope._attackMs),
			Variable("envelope.decayMs", 0, 1000, 0, _envelope._decayMs),
			Variable("envelope.sustain", 0, 127, 0, _envelope._sustain),
			Variable("envelope.releaseMs", 0, 1000, 0, _envelope._releaseMs)
		  })
		{}

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
				float freq = Utils::noteToFrequency(voice.note + _transpose + _pitchBendRange * pitchBend);
				float value = voice.oscillator.update(freq, timer);
				float env = _envelope.update(timer, voice.noteOnTime, voice.noteOffTime);

				v += Utils::volume(value * voice.velocity * env * amplitude());
			}

			StereoSample out = Utils::pan(v, panning());
			output(timer, out);
		}


	private:
		ADSR _envelope;
		int _amplitude;
		int _pitchBendRange;
		int _transpose;
		int _panning;
		Oscillator::Type _oscillatorType;

		float amplitude() {
			return _amplitude / 128.0;
		}

		float panning() {
			return _panning / 127.0;
		}

		std::vector<Voice> _voices;

		void removeFinishedVoices(const Timer &timer) {
			for (auto it = _voices.begin(); it != _voices.end();) {
				auto const &voice = *it;

				if (_envelope.isNoteDone(timer, voice.noteOffTime)) {
					it = _voices.erase(it);
				} else {
					++it;
				}
			}
		}
};
};

#endif
