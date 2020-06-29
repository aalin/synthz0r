#ifndef DEVICES__INSTRUMENTS__DRUMKIT_HPP
#define DEVICES__INSTRUMENTS__DRUMKIT_HPP

#include "../instrument_device.hpp"
#include "../../units/oscillator.hpp"
#include "../../units/adsr2.hpp"
#include "../../units/state_variable_filter.hpp"
#include "voices/voice_list.hpp"
#include "drums/kick.hpp"

namespace Devices::Instruments {
	class Drumkit : public InstrumentDevice {
		public:
			Drumkit();

			void handleEvents(const Transport &transport, const NoteEventList &events);

			StereoSample apply(const Transport &transport);

		private:
			Voices::VoiceList _voices;
			Drums::Kick _kick;

			Voices::VoicePtr makeVoice(uint8_t note) const;
	};
}

#endif
