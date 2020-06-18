#ifndef DEVICE_FACTORY_HPP
#define DEVICE_FACTORY_HPP

#include <list>
#include <string>
#include "devices/instrument_device.hpp"
#include "devices/effect_device.hpp"
#include "devices/note_device.hpp"

namespace DeviceFactory {
	typedef std::initializer_list<std::pair<std::string, int> > ParameterInitializerList;
	typedef std::initializer_list<std::pair<std::string, std::vector<int> > > TableInitializerList;

	Devices::InstrumentDevicePtr createInstrumentDevice(
		std::string name,
		ParameterInitializerList = {},
		TableInitializerList = {}
	);

	Devices::EffectDevicePtr createEffectDevice(
		std::string name,
		ParameterInitializerList,
		TableInitializerList
	);

	Devices::NoteDevicePtr createNoteDevice(
		std::string name,
		ParameterInitializerList = {},
		TableInitializerList = {}
	);

	const std::list<std::string> getInstrumentDeviceNames();
	const std::list<std::string> getEffectDeviceNames();
	const std::list<std::string> getNoteDeviceNames();
}

#endif
