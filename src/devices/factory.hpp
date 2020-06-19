#ifndef DEVICES__FACTORY_HPP
#define DEVICES__FACTORY_HPP

#include <list>
#include <string>
#include "instrument_device.hpp"
#include "effect_device.hpp"
#include "note_device.hpp"

namespace Devices::Factory {
	typedef std::initializer_list<std::pair<std::string, int> > ParameterInitializerList;
	typedef std::initializer_list<std::pair<std::string, std::vector<int> > > TableInitializerList;

	Devices::InstrumentDevicePtr createInstrumentDevice(
		std::string name,
		ParameterInitializerList = {},
		TableInitializerList = {}
	);

	Devices::EffectDevicePtr createEffectDevice(
		std::string name,
		ParameterInitializerList = {},
		TableInitializerList = {}
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
