#include "track.hpp"
#include <algorithm>

namespace Sequences {
	SequencePtr Track::insertSequence(uint32_t start, uint32_t length) {
		const auto it = std::find_if(
			_sequences.begin(),
			_sequences.end(),
			[&](const SequencePtr sequence) -> bool {
				return sequence->start() >= start;
			}
		);

		const SequencePtr sequence = std::make_shared<Sequence>(start, length);

		_sequences.insert(it, sequence);

		return sequence;
	}

	bool Track::eraseSequence(uint32_t position) {
		const auto it = std::find_if(
			_sequences.begin(),
			_sequences.end(),
			[&](const SequencePtr sequence) -> bool {
				return sequence->positionInside(position);
			}
		);

		if (it == _sequences.end()) {
			return false;
		}

		_sequences.erase(it);

		return true;
	}

	void Track::getEventsAt(std::list<NoteEvent> &events, uint32_t position) const {
		for (const SequencePtr sequence : _sequences) {
			if (sequence->positionInside(position)) {
				sequence->getEventsAt(events, position);
				return;
			}
		}
	}
}
