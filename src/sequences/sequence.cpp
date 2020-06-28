#include "sequence.hpp"
#include <algorithm>

namespace Sequences {
	void Sequence::insertNote(uint32_t start, uint32_t length, uint8_t value, uint8_t velocity) {
		const auto it = std::find_if(
			_notes.begin(),
			_notes.end(),
			[&](const Note &note) -> bool {
				return this->start() + note.start() >= start;
			}
		);

		_notes.insert(it, { start - this->start(), length, value, velocity });
	}

	bool Sequence::eraseNote(const uint32_t &position, const uint8_t &value) {
		const auto it = std::find_if(
			_notes.begin(),
			_notes.end(),
			[&](const Note &note) -> bool {
				return
					value == note.value() &&
					position >= start() + note.start() &&
					position < start() + note.end();
			});

		if (it == _notes.end()) {
			return false;
		}

		_notes.erase(it);

		return true;
	}

	void Sequence::getEventsAt(std::list<NoteEvent> &events, uint32_t position) const {
		for (const auto &note : _notes) {
			if (position == start() + note.start()) {
				events.push_back(NoteEvent::noteOn(note.value(), note.velocity()));
			}

			if (position == start() + note.end()) {
				events.push_back(NoteEvent::noteOff(note.value(), note.velocity()));
			}
		}
	}
}
