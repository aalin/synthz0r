#ifndef DEVICES__INSTRUMENTS__DRUMS__SNARE_HPP
#define DEVICES__INSTRUMENTS__DRUMS__SNARE_HPP

#include <vector>
#include "../voices/abstract_voice.hpp"
#include "../../../units/adsr2.hpp"
#include "../../../units/state_variable_filter.hpp"
#include "../../../units/oscillator.hpp"


namespace Devices::Instruments::Drums {
	class Snare {
		class Voice : public Voices::AbstractVoice {
			public:
				Voice(const Units::ADSR2::Settings &noiseEnvSettings, const Units::ADSR2::Settings &toneEnvSettings, float pitch);

				float update(const Transport &transport, const Voices::VoiceData &voiceData, const float &transpose);

				bool hasStopped() {
					return _noiseEnv.isOff() && _toneEnv.isOff();
				}

			private:
				Units::ADSR2 _noiseEnv;
				Units::ADSR2 _toneEnv;
				Units::Oscillator _noiseOsc;
				Units::Oscillator _toneOsc;
				Units::StateVariableFilter _noiseFilter;
				const float _pitch;
		};

		public:
			Snare();

			Voices::VoicePtr makeVoice() const {
				return std::make_unique<Voice>(
					noiseEnvSettings,
					toneEnvSettings,
					static_cast<float>(pitch)
				);
			}

			int pitch;
			Units::ADSR2::Settings noiseEnvSettings;
			Units::ADSR2::Settings toneEnvSettings;
	};
}

#endif
