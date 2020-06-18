#include "device_factory.hpp"

#include "devices/effects/bitcrusher.hpp"
#include "devices/effects/delay.hpp"
#include "devices/effects/overdrive.hpp"
#include "devices/instruments/kickdrum.hpp"
#include "devices/instruments/synth.hpp"
#include "devices/instruments/wavetable_synth.hpp"
#include "devices/notes/sequencer.hpp"

template<typename T> Devices::NoteDevicePtr makeNoteDevice() { return std::make_shared<T>(); }
template<typename T> Devices::EffectDevicePtr makeEffectDevice() { return std::make_shared<T>(); }
template<typename T> Devices::InstrumentDevicePtr makeInstrumentDevice() { return std::make_shared<T>(); }

#define EFFECT_HANDLER(NAME) {#NAME, makeEffectDevice<Devices::Effects::NAME>}
#define NOTE_HANDLER(NAME) {#NAME, makeNoteDevice<Devices::Notes::NAME>}
#define INSTRUMENT_HANDLER(NAME) {#NAME, makeInstrumentDevice<Devices::Instruments::NAME>}

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

const std::list<std::string> DeviceFactory::getInstrumentDeviceNames() {
	return extractKeys(InstrumentDeviceHandlers);
}

const std::list<std::string> DeviceFactory::getEffectDeviceNames() {
	return extractKeys(EffectDeviceHandlers);
}

const std::list<std::string> DeviceFactory::getNoteDeviceNames() {
	return extractKeys(NoteDeviceHandlers);
}

void applyParamsAndTables(
	Devices::DevicePtr device,
	DeviceFactory::ParameterInitializerList &params,
	DeviceFactory::TableInitializerList &tables
) {
	for (const auto &param : params) {
		device->setParam(param.first, param.second);
	}

	for (const auto &table : tables) {
		device->setTable(table.first, table.second);
	}
}

Devices::InstrumentDevicePtr DeviceFactory::createInstrumentDevice(
	std::string name,
	ParameterInitializerList params,
	TableInitializerList tables
) {
	const auto handler = InstrumentDeviceHandlers.find(name);

	if (handler == InstrumentDeviceHandlers.end()) {
		std::cerr << "Could not get instrument named " << name << std::endl;
		return nullptr;
	}

	auto device = handler->second();

	applyParamsAndTables(device, params, tables);

	return device;
}

Devices::EffectDevicePtr DeviceFactory::createEffectDevice(
	std::string name,
	ParameterInitializerList params,
	TableInitializerList tables
) {
	const auto handler = EffectDeviceHandlers.find(name);

	if (handler == EffectDeviceHandlers.end()) {
		std::cerr << "Could not get effect named " << name << std::endl;
		return nullptr;
	}

	auto device = handler->second();

	applyParamsAndTables(device, params, tables);

	return device;
}

Devices::NoteDevicePtr DeviceFactory::createNoteDevice(
	std::string name,
	ParameterInitializerList params,
	TableInitializerList tables
) {
	const auto handler = NoteDeviceHandlers.find(name);

	if (handler == NoteDeviceHandlers.end()) {
		std::cerr << "Could not get note device named " << name << std::endl;
		return nullptr;
	}

	auto device = handler->second();

	applyParamsAndTables(device, params, tables);

	return device;
}
