#include "drumkit.hpp"

namespace Devices::Instruments {
	Drumkit::Drumkit() : InstrumentDevice("Drumkit") {
		setupParameters({
			Parameter("kick.pitch",            1,   1600,   300, _kick.pitch),
		});
	}

	void Drumkit::handleEvents(const Transport &transport, const NoteEventList &events) {
		for (const auto &event : events) {
			switch (event.type) {
				case NoteEvent::Type::PAUSE_ALL:
					break;
				case NoteEvent::Type::NOTE_ON: {
					std::cout << "Playing " << event << std::endl;
					Voices::VoicePtr voice = makeVoice(event.note);

					if (voice == nullptr) {
						break;
					}

					_voices.play(
						transport,
						event.note,
						event.velocity / 128.f,
						std::move(voice)
					);

					break;
				}
				case NoteEvent::Type::NOTE_OFF:
					break;
				default:
					break;
			}
		}
	}

	Voices::VoicePtr Drumkit::makeVoice(uint8_t note) const {
		switch (note % 12) {
			case 0:
				return _kick.makeVoice();
			case 1:
				return _snare.makeVoice();
			case 2:
				return _hihat.makeVoice();
			default:
				return nullptr;
		}
	}

	StereoSample Drumkit::apply(const Transport &transport) {
		float v = _voices.update(transport, 0.0f);
		return StereoSample(v);
	}
}
