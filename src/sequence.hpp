#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <cstdint>
#include <list>
#include <memory>
#include "identifier.hpp"
#include "note_event.hpp"

class Sequence {
	public:
		class Note {
			public:
				Note(uint32_t start, uint32_t length, uint8_t value, uint8_t velocity)
				: _start(start),
				  _length(length),
				  _value(value),
				  _velocity(velocity)
				{}

				uint32_t start()    const { return _start; }
				uint32_t length()   const { return _length; }
				uint32_t end()      const { return _start + _length; }
				uint8_t  value()    const { return _value; }
				uint8_t  velocity() const { return _velocity; }

			private:
				uint32_t _start;
				uint32_t _length;
				uint8_t _value;
				uint8_t _velocity;
		};

		Sequence(uint32_t start, uint32_t length)
		: _start(start),
		  _length(length)
		{}

		uint32_t id() {
			return _id;
		}

		void insertNote(uint32_t start, uint32_t length, uint8_t value, uint8_t velocity);
		bool eraseNote(const uint32_t &position, const uint8_t &value);

		const std::list<Note> getNotes() const {
			return _notes;
		}

		uint32_t start() const {
			return _start;
		}

		uint32_t length() const {
			return _length;
		}

		uint32_t end() const {
			return _start + _length;
		}

		bool positionInside(uint32_t position) const {
			return position >= start() && position < end();
		}

		void getEventsAt(std::list<NoteEvent> &events, uint32_t position) const;

	private:
		const Identifier _id;
		std::list<Note> _notes;

		uint32_t _start;
		uint32_t _length;
};

typedef std::shared_ptr<Sequence> SequencePtr;

#endif
