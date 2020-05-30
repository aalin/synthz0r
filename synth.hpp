#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "oscillator.hpp"
#include <map>

class Synth {
	public:
		struct Voice {
				Voice(float vol) : volume(vol) { }
				float volume;
				Oscillator oscillator;
			};

			void addVoice(int note, float volume = 1.0) {
				_voices.insert(std::pair(note, Voice(volume)));
			}

			void removeVoice(int note) {
				_voices.erase(note);
			}

			float mix(float sampleRate) {
				float sum = 0.0;

				for (auto & [note, voice] : _voices) {
					float freq = noteToFrequency(note);
					sum += voice.oscillator.sine(freq, sampleRate);
				}

				return sum;
			}

	private:
		std::map<int, Voice> _voices;

		float noteToFrequency(float note) {
			return std::pow(2, (note - 69) / 12) * 440.0;
		}
};

#endif
