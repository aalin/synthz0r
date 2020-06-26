#ifndef DEVICES__INSTRUMENTS__VOICES__VOICE_LIST_HPP
#define DEVICES__INSTRUMENTS__VOICES__VOICE_LIST_HPP

#include <list>
#include "../../../transport.hpp"
#include "abstract_voice.hpp"
#include "voice_data.hpp"

namespace Devices::Instruments::Voices {
	class VoiceList {
		public:
			void play(const Transport &transport, const uint8_t &note, const float &velocity, VoicePtr voice) {
				VoiceData voiceData(
					transport,
					note,
					velocity
				);

				_voices.push_back(
					std::make_pair<VoiceData, VoicePtr>(
						std::move(voiceData),
						std::move(voice)
					)
				);
			}

			void stop(const Transport &transport, uint8_t note) {
				for (auto &v : _voices) {
					if (v.first.note() == note && v.first.noteOffTime() < 0.0) {
						v.first.stopAt(transport.secondsElapsedSinceStart());
					}
				}
			}

			void stopAll(const Transport &transport) {
				for (auto &voice : _voices) {
					voice.first.stopAt(transport.secondsElapsedSinceStart());
				}
			}

			float update(const Transport &transport, float transpose) {
				float sum = 0.0;

				for (auto &voice : _voices) {
					sum += voice.second->update(transport, voice.first, transpose);
				}

				clearStopped();

				return sum;
			}

		private:
			std::list<std::pair<VoiceData, VoicePtr> > _voices;

			void clearStopped() {
				for (auto it = _voices.begin(); it != _voices.end();) {
					if (it->second->hasStopped()) {
						it = _voices.erase(it);
					} else {
						++it;
					}
				}
			}
	};
}

#endif
