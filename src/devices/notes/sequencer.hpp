#ifndef DEVICES__NOTES__SEQUENCER_HPP
#define DEVICES__NOTES__SEQUENCER_HPP

#include <vector>
#include "../note_device.hpp"

namespace Devices {
	namespace Notes {
		class Sequencer : public NoteDevice {
			public:
				Sequencer();

				void apply(const Timer &, NoteEventList &);

			private:
				int _steps;
				int _velocity;
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
}

#endif
