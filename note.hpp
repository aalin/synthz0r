#ifndef NOTE_HPP
#define NOTE_HPP

enum NoteValue {
	C  = 0,
	Db = 1,
	D  = 2,
	Eb = 3,
	E  = 4,
	F  = 5,
	Gb = 6,
	G  = 7,
	Ab = 8,
	A  = 9,
	Bb = 10,
	B  = 11
};

template<NoteValue V, int8_t O>
struct Note {
	static constexpr int8_t value = (O + 1) * 12 + V;
};

#define NOTE(VALUE, OCTAVE) Note<NoteValue::VALUE,OCTAVE>::value
#define NOTE_OFF -1

#endif
