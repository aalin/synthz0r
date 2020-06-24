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
