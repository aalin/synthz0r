#ifndef DEVICES__INSTRUMENTS__DRUMKIT_HPP
#define DEVICES__INSTRUMENTS__DRUMKIT_HPP

#include "../instrument_device.hpp"
#include "../../units/oscillator.hpp"
#include "../../units/adsr2.hpp"
#include "../../units/state_variable_filter.hpp"
#include "voices/voice_list.hpp"
#include "drums/kick.hpp"
#include "drums/hihat.hpp"
#include "drums/snare.hpp"

namespace Devices::Instruments {
	class Drumkit : public InstrumentDevice {
		public:
			Drumkit();

			void handleEvents(const Transport &transport, const NoteEventList &events);

			StereoSample apply(const Transport &transport);

		private:
			int _amplitude;

			float amplitude() const {
				return _amplitude / 100.0;
			}

			Voices::VoiceList _voices;
			Drums::Kick _kick;
			Drums::Hihat _hihat;
			Drums::Snare _snare;

			Voices::VoicePtr makeVoice(uint8_t note) const;
	};
}

#endif
