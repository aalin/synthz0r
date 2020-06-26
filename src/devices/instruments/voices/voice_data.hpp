#ifndef DEVICES__INSTRUMENTS__VOICES__VOICE_DATA_HPP
#define DEVICES__INSTRUMENTS__VOICES__VOICE_DATA_HPP

#include "../../../transport.hpp"

namespace Devices::Instruments::Voices {
	class VoiceData {
		public:
			VoiceData(const Transport &transport, const uint8_t &note, const float &velocity)
			: _note(note),
			  _velocity(velocity),
			  _noteOnTime(transport.secondsElapsedSinceStart()),
			  _noteOffTime(-1.0)
			{}

			const uint8_t & note() const {
				return _note;
			}

			const float & velocity() const {
				return _velocity;
			}

			const float & noteOnTime() const {
				return _noteOnTime;
			}

			const float & noteOffTime() const {
				return _noteOffTime;
			}

			void stopAt(float time) {
				_noteOffTime = time;
			}

		private:
			uint8_t _note;
			float _velocity;
			float _noteOnTime;
			float _noteOffTime;
	};
}

#endif
