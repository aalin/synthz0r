#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <map>
#include "oscillator.hpp"
#include "device.hpp"

class Synth : public Device {
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

			float update(float time, float sampleRate) {
				float sum = 0.0;

				for (auto & [note, voice] : _voices) {
					float freq = noteToFrequency(note);
					sum += voice.oscillator.update(freq, sampleRate);
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
