#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <cstdint>
#include <list>
#include "identifier.hpp"
#include "note_event.hpp"

class Sequence {
	public:
		struct Note {
			uint64_t start;
			uint64_t length;
			uint8_t value;
			uint8_t velocity;
		};

		Sequence(uint64_t start, uint64_t length)
		: _start(start),
		  _length(length)
		{}

		uint32_t id() {
			return _id;
		}

		void insertNote(uint64_t start, uint64_t length, uint8_t value, uint8_t velocity);
		bool eraseNote(const uint64_t &position, const uint8_t &value);

		const std::list<Note> getNotes() const {
			return _notes;
		}

		uint64_t start() const {
			return _start;
		}

		uint64_t length() const {
			return _length;
		}

		uint64_t end() const {
			return _start + _length;
		}

		bool positionInside(uint64_t position) const {
			return position >= start() && position < end();
		}

		void getEventsAt(std::list<NoteEvent> &events, uint64_t position) const;

	private:
		const Identifier _id;
		std::list<Note> _notes;

		uint64_t _start;
		uint64_t _length;
};

#endif
