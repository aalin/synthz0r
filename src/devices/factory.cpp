#include "factory.hpp"

#include "effects/bitcrusher.hpp"
#include "effects/delay.hpp"
#include "effects/overdrive.hpp"
#include "instruments/kickdrum.hpp"
#include "instruments/synth.hpp"
#include "instruments/wavetable_synth.hpp"
#include "notes/sequencer.hpp"

using Devices::Factory::ParameterInitializerList;
using Devices::Factory::TableInitializerList;

template<typename DeviceType, typename PtrType>
PtrType makeDevice() {
	return std::make_shared<DeviceType>();
}

#define EFFECT_HANDLER(NAME) {#NAME, makeDevice<Devices::Effects::NAME, Devices::EffectDevicePtr>}
#define NOTE_HANDLER(NAME) {#NAME, makeDevice<Devices::Notes::NAME, Devices::NoteDevicePtr>}
#define INSTRUMENT_HANDLER(NAME) {#NAME, makeDevice<Devices::Instruments::NAME, Devices::InstrumentDevicePtr>}

static const std::map<std::string, std::function<Devices::EffectDevicePtr()>> EffectDeviceHandlers = {
	EFFECT_HANDLER(Bitcrusher),
	EFFECT_HANDLER(Delay),
	EFFECT_HANDLER(Overdrive),
};

static const std::map<std::string, std::function<Devices::InstrumentDevicePtr()> > InstrumentDeviceHandlers = {
	INSTRUMENT_HANDLER(Synth),
	INSTRUMENT_HANDLER(WavetableSynth),
	INSTRUMENT_HANDLER(Kickdrum),
};

static const std::map<std::string, std::function<Devices::NoteDevicePtr()> > NoteDeviceHandlers = {
	NOTE_HANDLER(Sequencer),
};

template<typename MapType>
std::list<std::string> extractKeys(const MapType &map) {
	std::list<std::string> keys;

	for (const auto &elem : map) {
		keys.push_back(elem.first);
	}

	return keys;
}

const std::list<std::string> Devices::Factory::getInstrumentDeviceNames() {
	return extractKeys(InstrumentDeviceHandlers);
}

const std::list<std::string> Devices::Factory::getEffectDeviceNames() {
	return extractKeys(EffectDeviceHandlers);
}

const std::list<std::string> Devices::Factory::getNoteDeviceNames() {
	return extractKeys(NoteDeviceHandlers);
}

void applyParamsAndTables(
	Devices::DevicePtr device,
	ParameterInitializerList &params,
	TableInitializerList &tables
) {
	for (const auto &param : params) {
		device->setParam(param.first, param.second);
	}

	for (const auto &table : tables) {
		device->setTable(table.first, table.second);
	}
}

template<typename PtrType, typename MapType>
PtrType instantiateDevice(
	const MapType &handlerMap,
	std::string name,
	ParameterInitializerList &params,
	TableInitializerList &tables
) {
	const auto handler = handlerMap.find(name);

	if (handler == handlerMap.end()) {
		std::cerr << "Could not get handler for " << name << ", returning nullptr" << std::endl;
		return nullptr;
	}

	auto device = handler->second();

	applyParamsAndTables(device, params, tables);

	return device;
}

Devices::InstrumentDevicePtr Devices::Factory::createInstrumentDevice(
	std::string name,
	ParameterInitializerList params,
	TableInitializerList tables
) {
	return instantiateDevice<Devices::InstrumentDevicePtr>(
		InstrumentDeviceHandlers,
		name,
		params,
		tables
	);
}

Devices::EffectDevicePtr Devices::Factory::createEffectDevice(
	std::string name,
	ParameterInitializerList params,
	TableInitializerList tables
) {
	return instantiateDevice<Devices::EffectDevicePtr>(
		EffectDeviceHandlers,
		name,
		params,
		tables
	);
}

Devices::NoteDevicePtr Devices::Factory::createNoteDevice(
	std::string name,
	ParameterInitializerList params,
	TableInitializerList tables
) {
	return instantiateDevice<Devices::NoteDevicePtr>(
		NoteDeviceHandlers,
		name,
		params,
		tables
	);
}
