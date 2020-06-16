#include "sequencer.hpp"

void Devices::Sequencer::update(const Timer &timer, float) {
	 if (static_cast<size_t>(_steps) != _notes.size()) {
		 _notes.resize(_steps, -1);
	 }

	 const float speed = 60.0 / _bpm;
	 const int step = Utils::mod(static_cast<size_t>(timer.seconds() / speed), _notes.size());

	 if (step != _lastStep) {
		 std::cout << "Sequencer update: " << step << std::endl;
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
