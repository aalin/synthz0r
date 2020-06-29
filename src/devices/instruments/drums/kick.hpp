#ifndef DEVICES__INSTRUMENTS__DRUMS__KICK_HPP
#define DEVICES__INSTRUMENTS__DRUMS__KICK_HPP

#include "../voices/abstract_voice.hpp"
#include "../../../units/adsr2.hpp"
#include "../../../units/oscillator.hpp"

namespace Devices::Instruments::Drums {
	class Kick {
		class Voice : public Voices::AbstractVoice {
			public:
				Voice(const Units::ADSR2::Settings &pitchEnvelopeSettings, const Units::ADSR2::Settings &volumeEnvelopeSettings, float pitch);

				float update(const Transport &transport, const Voices::VoiceData &voiceData, const float &transpose);

				bool hasStopped() {
					return _volumeEnvelope.isOff();
				}

			private:
				Units::ADSR2 _pitchEnvelope;
				Units::ADSR2 _volumeEnvelope;
				Units::Oscillator _oscillator;
				const float _pitch;
		};

		public:
			Kick();

			Voices::VoicePtr makeVoice() const {
				return std::make_unique<Voice>(
					pitchEnvelopeSettings,
					volumeEnvelopeSettings,
					static_cast<float>(pitch)
				);
			}

			int pitch;
			Units::ADSR2::Settings pitchEnvelopeSettings;
			Units::ADSR2::Settings volumeEnvelopeSettings;
	};
}

#endif
