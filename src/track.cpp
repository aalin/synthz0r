#include "track.hpp"
#include <algorithm>

void Track::insertSequence(uint64_t start, uint64_t length) {
	const auto it = std::find_if(
		_sequences.begin(),
		_sequences.end(),
		[&](const Sequence &sequence) -> bool {
			return sequence.start() >= start;
		}
	);

	_sequences.insert(it, { start, length });
}

bool Track::eraseSequence(uint64_t position) {
	const auto it = std::find_if(
		_sequences.begin(),
		_sequences.end(),
		[&](const Sequence &sequence) -> bool {
			return sequence.positionInside(position);
		}
	);

	if (it == _sequences.end()) {
		return false;
	}

	_sequences.erase(it);

	return true;
}

void Track::getEventsAt(std::list<NoteEvent> &events, uint64_t position) const {
	for (const auto &sequence : _sequences) {
		if (sequence.positionInside(position)) {
			sequence.getEventsAt(events, position);
			return;
		}
	}
}
