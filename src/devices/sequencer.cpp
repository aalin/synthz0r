#include "sequencer.hpp"

void Devices::Sequencer::update(const Timer &timer, float) {
	if (_notes.empty()) {
		return;
	}

	const double beatsPerSecond = (_bpm * _rate) / 60.0;
	const double speed = beatsPerSecond / timer.sampleRate();
	const int step = Utils::mod(static_cast<size_t>(_time), _notes.size());

	_time += speed;

	if (step != _lastStep) {
		if (_lastNote >= 0) {
			output(timer, NoteEvent::noteOff(_lastNote));
		}

		if (_notes[step] >= 0) {
			output(timer, NoteEvent::noteOn(_notes[step], _velocity));
		}

		_lastNote = _notes[step];
		_lastStep = step;
	}
}
