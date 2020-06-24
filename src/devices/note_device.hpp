#ifndef DEVICES__NOTE_DEVICE_HPP
#define DEVICES__NOTE_DEVICE_HPP

#include "base_device.hpp"
#include "../note_event.hpp"

namespace Devices {
	class NoteDevice : public BaseDevice {
		public:
			NoteDevice(std::string name) : BaseDevice(name) {}

			virtual ~NoteDevice() {}

			virtual void apply(const Transport &, NoteEventList &) = 0;
	};

	typedef std::shared_ptr<NoteDevice> NoteDevicePtr;
}

#endif
