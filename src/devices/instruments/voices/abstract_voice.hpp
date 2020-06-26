#ifndef DEVICES__INSTRUMENTS__VOICES__ABSTRACT_VOICE_HPP
#define DEVICES__INSTRUMENTS__VOICES__ABSTRACT_VOICE_HPP

#include "../../../transport.hpp"
#include "voice_data.hpp"

namespace Devices::Instruments::Voices {
	class AbstractVoice {
		public:
			virtual ~AbstractVoice() {}
			virtual float update(const Transport &, const VoiceData&, const float &) = 0;
			virtual bool hasStopped() = 0;
	};

	typedef std::unique_ptr<AbstractVoice> VoicePtr;
}

#endif
