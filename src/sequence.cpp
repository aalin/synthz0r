#include "sequence.hpp"
#include <algorithm>

void Sequence::insertNote(uint64_t start, uint64_t length, uint8_t value, uint8_t velocity) {
	const auto it = std::find_if(
		_notes.begin(),
		_notes.end(),
		[&](const Note &note) -> bool {
			return note.start >= start;
		}
	);

	_notes.insert(it, { start, length, value, velocity });
}

bool Sequence::eraseNote(const uint64_t &position, const uint8_t &value) {
	const auto it = std::find_if(
		_notes.begin(),
		_notes.end(),
		[&](const Note &note) -> bool {
			return
				note.value == value &&
				position >= note.start &&
				position <= (note.start + note.length);
		});

	if (it == _notes.end()) {
		return false;
	}

	_notes.erase(it);

	return true;
}

void Sequence::getEventsAt(std::list<NoteEvent> &events, uint64_t position) const {
	for (const auto &note : _notes) {
		if (position == note.start) {
			events.push_back(NoteEvent::noteOn(note.value, note.velocity));
			return;
		}

		if (position == note.start + note.length) {
			events.push_back(NoteEvent::noteOff(note.value, note.velocity));
			return;
		}
	}
}
