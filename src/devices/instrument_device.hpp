#ifndef DEVICES__INSTRUMENT_DEVICE_HPP
#define DEVICES__INSTRUMENT_DEVICE_HPP

#include "base_device.hpp"
#include "../stereo_sample.hpp"
#include "../note_event.hpp"

namespace Devices {
	class InstrumentDevice : public BaseDevice {
		public:
			InstrumentDevice(
				std::string name,
				std::initializer_list<Parameter> params = {},
				std::initializer_list<Table> tables = {}
			)
			: BaseDevice(name, params, tables)
			{}

			virtual ~InstrumentDevice() {}

			virtual StereoSample apply(const Timer &, const NoteEventList &) = 0;
	};

	typedef std::shared_ptr<InstrumentDevice> InstrumentDevicePtr;
}

#endif
