#include "sequencer.hpp"

namespace Devices::Notes {
	void Sequencer::apply(const Timer &timer, NoteEventList &events) {
		if (_notes.empty()) {
			return;
		}

		const int step = Utils::mod(static_cast<size_t>(timer.beat()), _notes.size());

		if (step != _lastStep) {
			if (_lastNote >= 0) {
				events.push_back(NoteEvent::noteOff(_lastNote));
			}

			if (_notes[step] >= 0) {
				std::cout << "Sequencer triggering note " << _notes[step] << std::endl;
				events.push_back(NoteEvent::noteOn(_notes[step], _velocity));
			}

			_lastNote = _notes[step];
			_lastStep = step;
		}
	}
}
