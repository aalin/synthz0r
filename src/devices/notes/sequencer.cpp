#include "sequencer.hpp"
#include <cmath>

constexpr static uint8_t INITIAL_STEPS = 8;
constexpr static uint8_t INITIAL_VELOCITY = 100;
constexpr static uint8_t INITIAL_BPM = 50;
constexpr static uint8_t INITIAL_RATE = 0;

namespace Devices::Notes {
	Sequencer::Sequencer() : NoteDevice("Sequencer") {
		setupParameters({
			Parameter("numSteps", 0,  32, INITIAL_STEPS,    _steps),
			Parameter("velocity", 0, 127, INITIAL_VELOCITY, _velocity),
			Parameter("rate",     0,   8, INITIAL_RATE,     _rate)
		});

		setupTables({
			Table("notes", -1, 127, -1, INITIAL_STEPS, _notes)
		});
	}

	void Sequencer::apply(const Timer &timer, NoteEventList &events) {
		if (_notes.empty()) {
			return;
		}

		const int step = Utils::mod(static_cast<size_t>(timer.position().total4ths() * std::pow(2, _rate)), _notes.size());

		if (step != _lastStep) {
			std::cout << "Step:" << step << std::endl;

			if (_lastNote >= 0) {
				events.push_back(NoteEvent::noteOff(_lastNote));
				std::cout << "Sequencer " << id() << " event at " << timer.position() << ": " << NoteEvent::noteOff(_lastNote) << std::endl;
			}

			if (_notes[step] >= 0) {
				std::cout << "Sequencer " << id() << " event at " << timer.position() << ": " << NoteEvent::noteOn(_notes[step], _velocity) << std::endl;
				events.push_back(NoteEvent::noteOn(_notes[step], _velocity));
			}

			_lastNote = _notes[step];
			_lastStep = step;
		}
	}
}
