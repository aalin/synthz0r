#ifndef DEVICES__NOTE_DEVICE_HPP
#define DEVICES__NOTE_DEVICE_HPP

#include "base_device.hpp"
#include "../note_event.hpp"

namespace Devices {
	class NoteDevice : public BaseDevice {
		public:
			NoteDevice(
				std::string name,
				std::initializer_list<Parameter> params = {},
				std::initializer_list<Table> tables = {}
			)
			: BaseDevice(name, params, tables)
			{}

			virtual ~NoteDevice() {}

			virtual void apply(const Timer &, NoteEventList &) = 0;
	};

	typedef std::shared_ptr<NoteDevice> NoteDevicePtr;
}

#endif
