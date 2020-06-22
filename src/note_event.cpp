#include "note_event.hpp"
#include <iostream>
#include <sstream>

std::string noteString(uint8_t value) {
	std::ostringstream oss;

	const int note = value % 12;
	const int octave = value / 12;

	switch (note) {
		case 0: oss << "C"; break;
		case 1: oss << "Db"; break;
		case 2: oss << "D"; break;
		case 3: oss << "Eb"; break;
		case 4: oss << "E"; break;
		case 5: oss << "F"; break;
		case 6: oss << "Gb"; break;
		case 7: oss << "G"; break;
		case 8: oss << "Ab"; break;
		case 9: oss << "A"; break;
		case 10: oss << "Bb"; break;
		case 11: oss << "B"; break;
		default: break;
	}

	oss << octave;

	return oss.str();
}

std::ostream & operator<<(std::ostream &out, const NoteEvent &noteEvent) {
	return out <<
		"NoteEvent(" <<
		(noteEvent.type == NoteEvent::Type::NOTE_ON ? "NOTE_ON" : "NOTE_OFF") <<
		" " << noteString(noteEvent.note) << ")";
}
