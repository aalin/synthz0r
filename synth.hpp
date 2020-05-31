#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <map>
#include <list>
#include "oscillator.hpp"
#include "device.hpp"
#include "adsr.hpp"

class Synth : public Device {
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
			: Device(),
			  pitchBendRange(2.0),
			  transpose(0),
			  _oscillatorType(oscillatorType)
			{}

			void noteOn(int note, float velocity = 2.0) {
				_voices.push_back(Voice(_oscillatorType, note, velocity, _time));
			}

			void noteOff(int note) {
				for (auto &v : _voices) {
					if (v.note == note && v.noteOffTime < 0.0) {
						v.noteOffTime = _time;
					}
				}
			}

			float tick(float time, float _sampleRate) {
				_time = time;
			}

			float update(float time, float sampleRate, float pitchBend = 0.0) {
				float sum = 0.0;

				for (auto it = _voices.begin(); it != _voices.end();) {
					auto const &voice = *it;

					if (envelope.isNoteDone(time, voice.noteOffTime)) {
						_voices.erase(it);
					} else {
						++it;
					}
				}

				for (auto &voice : _voices) {
					float freq = noteToFrequency(voice.note + transpose + pitchBendRange * pitchBend);
					float value = voice.oscillator.update(freq, sampleRate);
					float env = envelope.getValue(time, voice.noteOnTime, voice.noteOffTime);

					sum += (std::exp(value * voice.velocity * env * amplitude) - 1) / (2.718281828459045 - 1);
				}

				return sum;
			}

			ADSR envelope;
			float amplitude;
			float pitchBendRange;
			int transpose;

	private:
		float _time;
		Oscillator::Type _oscillatorType;
		std::vector<Voice> _voices;

		float noteToFrequency(float note) {
			return std::pow(2, (note - 69) / 12) * 440.0;
		}
};

#endif
