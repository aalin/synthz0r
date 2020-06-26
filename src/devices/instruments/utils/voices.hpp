#ifndef DEVICES__INSTRUMENTS__UTILS__VOICES_HPP
#define DEVICES__INSTRUMENTS__UTILS__VOICES_HPP

#include <list>
#include "../../../transport.hpp"

struct VoiceData {
	VoiceData(uint8_t note, float velocity, float noteOnTime)
	: note(note),
	  velocity(velocity),
	  noteOnTime(noteOnTime),
	  noteOffTime(-1.0)
	{}

	uint8_t note;
	float velocity;
	float noteOnTime;
	float noteOffTime;
};

template<typename T>
class Voices {
	struct Voice {
		Voice(VoiceData data, T voice)
		: data(data), voice(voice)
		{}

		VoiceData data;
		T voice;

		float update(const Transport &transport, float transpose) {
			return voice.update(transport, data, transpose);
		}

		bool hasStopped() {
			return voice.hasStopped();
		}
	};

	public:
		void play(const Transport &transport, uint8_t note, float velocity, T voice) {
			Voice v(
				VoiceData(
					note,
					velocity,
					transport.secondsElapsedSinceStart()
				),
				voice
			);

			_voices.push_back(v);
		}

		void stop(const Transport &transport, uint8_t note) {
			for (auto &v : _voices) {
				if (v.data.note == note && v.data.noteOffTime < 0.0) {
					v.data.noteOffTime = transport.secondsElapsedSinceStart();
				}
			}
		}

		void stopAll(const Transport &transport) {
			for (auto &v : _voices) {
				v.data.noteOffTime = transport.secondsElapsedSinceStart();
			}
		}

		float update(const Transport &transport, float transpose) {
			float sum = 0.0;

			for (auto &voice : _voices) {
				sum += voice.update(transport, transpose);
			}

			clearStopped();

			return sum;
		}

	private:
		std::list<Voice> _voices;

		void clearStopped() {
			size_t stoppedCount = 0;

			for (auto it = _voices.begin(); it != _voices.end();) {
				if (it->hasStopped()) {
					stoppedCount++;
					it = _voices.erase(it);
				} else {
					++it;
				}
			}

			if (stoppedCount > 0) {
				std::cout << "Stopped " << stoppedCount << " voices" << std::endl;
			}
		}

};

#endif
