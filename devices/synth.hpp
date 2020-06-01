#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <map>
#include <list>
#include "base_device.hpp"
#include "../units/oscillator.hpp"
#include "../units/adsr.hpp"
#include "../effects/base_effect.hpp"
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
			: BaseDevice(),
			  pitchBendRange(2.0),
			  transpose(0),
			  _oscillatorType(oscillatorType),
			  _panning(0.0)
			{}

			const char * name() const { return "Synth"; }

			void addEffect(std::shared_ptr<Effects::BaseEffect> effect) {
				_effects.push_back(effect);
			}

			void noteOn(const Timer &timer, int note, float velocity = 2.0) {
				_voices.push_back(Voice(_oscillatorType, note, velocity, timer.getSeconds()));
			}

			void noteOff(const Timer &timer, int note) {
				for (auto &v : _voices) {
					if (v.note == note && v.noteOffTime < 0.0) {
						v.noteOffTime = timer.getSeconds();
					}
				}
			}

			void update(const Timer &timer, float pitchBend = 0.0) {
				StereoSample out;

				for (auto it = _voices.begin(); it != _voices.end();) {
					auto const &voice = *it;

					if (envelope.isNoteDone(timer, voice.noteOffTime)) {
						_voices.erase(it);
					} else {
						++it;
					}
				}

				for (auto &voice : _voices) {
					float freq = noteToFrequency(voice.note + transpose + pitchBendRange * pitchBend);
					float value = voice.oscillator.update(freq, timer);
					float env = envelope.update(timer, voice.noteOnTime, voice.noteOffTime);

					out.add(Utils::volume(value * voice.velocity * env * amplitude));
				}

				for (auto &effect : _effects) {
					out = effect->apply(out);
				}

				output(timer, out);
			}

			ADSR envelope;
			float amplitude;
			float pitchBendRange;
			int transpose;

	private:
		Oscillator::Type _oscillatorType;
		std::vector<Voice> _voices;
		std::vector<std::shared_ptr<Effects::BaseEffect> > _effects;
		float _panning;

		float noteToFrequency(float note) {
			return std::pow(2, (note - 69) / 12) * 440.0;
		}
};
};

#endif