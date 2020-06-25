#ifndef NOTE_EVENT_HPP
#define NOTE_EVENT_HPP

#include <list>
#include <ostream>

struct NoteEvent {
	enum Type {
		NOTE_ON,
		NOTE_OFF,
		PAUSE_ALL
	};

	static NoteEvent pauseAll() {
		return { PAUSE_ALL, 0, 0 };
	}

	static NoteEvent noteOn(uint8_t note, uint8_t velocity = 100) {
		return { NOTE_ON, note, velocity };
	}

	static NoteEvent noteOff(uint8_t note, uint8_t velocity = 0) {
		return { NOTE_OFF, note, velocity };
	}

	Type type;
	uint8_t note;
	uint8_t velocity;
};

std::ostream & operator<<(std::ostream &, const NoteEvent &);

typedef std::list<NoteEvent> NoteEventList;

#endif
