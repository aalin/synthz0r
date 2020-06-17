#ifndef DEVICES__SEQUENCER_HPP
#define DEVICES__SEQUENCER_HPP

#include <vector>
#include "base_device.hpp"

namespace Devices {
	class Sequencer : public BaseDevice {
		constexpr static uint8_t INITIAL_STEPS = 8;
		constexpr static uint8_t INITIAL_VELOCITY = 100;
		constexpr static uint8_t INITIAL_BPM = 50;
		constexpr static uint8_t INITIAL_RATE = 1;

		public:
			Sequencer()
			: BaseDevice("Sequencer", {
				Parameter("numSteps", 0,  32, INITIAL_STEPS,    _steps),
				Parameter("velocity", 0, 127, INITIAL_VELOCITY, _velocity),
				Parameter("bpm",      0, 400, INITIAL_BPM,      _bpm),
				Parameter("rate",     0,   8, INITIAL_RATE,     _rate)
			  }, {
				Table("notes", -1, 127, -1, INITIAL_STEPS, _notes)
			  })
			{}

			void update(const Timer &, float);

			Sequencer * setStep(size_t step, int8_t value) {
				std::cout << name() << ": attempting to set step " << step << " but size is " << _notes.size() << std::endl;
				if (_notes.size() < step) {
					std::cout << name() << ": attempted to set step " << step << " but size is " << _notes.size() << std::endl;
					return this;
				}

				_notes[step] = value;
				return this;
			}

			Sequencer * clearStep(size_t step) {
				_notes[step] = -1;
				return this;
			}

		private:
			int _steps;
			int _velocity;
			int _bpm;
			int _rate;

			int8_t _lastStep = -1;
			int8_t _lastNote = -1;
			double _time = 0.0;

			std::vector<int> _notes;

			void respondToParameterChange(const std::string &name) {
				if (name == "steps") {
					std::cout << "Steps changed to " << _steps << std::endl;

					 if (static_cast<size_t>(_steps) != _notes.size()) {
						 std::cout << "Resizing _notes" << std::endl;
						 _notes.resize(_steps, -1);
					 }
				}
			}
	};
}

#endif
