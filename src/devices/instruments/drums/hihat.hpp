#ifndef DEVICES__INSTRUMENTS__DRUMS__HIHAT_HPP
#define DEVICES__INSTRUMENTS__DRUMS__HIHAT_HPP

#include <vector>
#include "../voices/abstract_voice.hpp"
#include "../../../units/adsr2.hpp"
#include "../../../units/state_variable_filter.hpp"
#include "../../../units/oscillator.hpp"


namespace Devices::Instruments::Drums {
	class Hihat {
		// Based on http://joesul.li/van/synthesizing-hi-hats/
		class Voice : public Voices::AbstractVoice {
			public:
				Voice(const Units::ADSR2::Settings &volumeEnvelopeSettings, float pitch);

				float update(const Transport &transport, const Voices::VoiceData &voiceData, const float &transpose);

				bool hasStopped() {
					return _volumeEnvelope.isOff();
				}

			private:
				Units::ADSR2 _volumeEnvelope;
				Units::StateVariableFilter _bandpass;
				Units::StateVariableFilter _highpass;
				const float _pitch;
				std::vector<Units::Oscillator> _oscillators;
		};

		public:
			Hihat();

			Voices::VoicePtr makeVoice(float decayRatio) const {
				Units::ADSR2::Settings volumeEnvSettings = volumeEnvelopeSettings;
				volumeEnvSettings._decayMs *= decayRatio;

				return std::make_unique<Voice>(
					volumeEnvSettings,
					static_cast<float>(pitch)
				);
			}

			int pitch;
			Units::ADSR2::Settings volumeEnvelopeSettings;
	};
}

#endif
