#ifndef DEVICES__SEQUENCER_HPP
#define DEVICES__SEQUENCER_HPP

#include <vector>
#include "base_device.hpp"

namespace Devices {
	class Sequencer : public BaseDevice {
		public:
			Sequencer(uint8_t steps = 16, uint8_t velocity = 100, uint8_t bpm = 100)
			: BaseDevice("Sequencer", {
				Parameter("steps",    0,  32,    steps,    _steps),
				Parameter("velocity", 0, 127, velocity, _velocity),
				Parameter("bpm",      0, 400,      bpm,      _bpm)
			  }),
			  _notes(steps, -1)
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

			int8_t _lastStep = -1;
			int8_t _lastNote = -1;

			std::vector<int8_t> _notes;
	};
}

#endif
