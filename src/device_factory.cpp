#include "device_factory.hpp"

#include "devices/bitcrusher.hpp"
#include "devices/delay.hpp"
#include "devices/kickdrum.hpp"
#include "devices/overdrive.hpp"
#include "devices/sequencer.hpp"
#include "devices/synth.hpp"
#include "devices/wavetable_synth.hpp"

template<typename T>
Devices::DevicePtr buildDevice() {
	return std::make_shared<T>();
}

#define HANDLER(NAME) {#NAME, buildDevice<Devices::NAME>}

static const std::map<std::string, std::function<Devices::DevicePtr()> > Handlers = {
	HANDLER(Bitcrusher),
	HANDLER(Delay),
	HANDLER(Kickdrum),
	HANDLER(Overdrive),
	HANDLER(Sequencer),
	HANDLER(Synth),
	HANDLER(WavetableSynth),
};

const std::list<std::string> DeviceFactory::getDeviceNames() {
	std::list<std::string> names;

	for (const auto &handler : Handlers) {
		names.push_back(handler.first);
	}

	return names;
}

Devices::DevicePtr DeviceFactory::create(std::string name) {
	const auto handler = Handlers.find(name);

	if (handler == Handlers.end()) {
		return nullptr;
	}

	return handler->second();
}

Devices::DevicePtr DeviceFactory::create(std::string name, ParameterInitializerList params) {
	Devices::DevicePtr device = create(name);

	for (const auto param : params) {
		device->setParam(param.first, param.second);
	}

	return device;
}

Devices::DevicePtr DeviceFactory::create(std::string name, ParameterInitializerList params, TableInitializerList tables) {
	Devices::DevicePtr device = create(name, params);

	for (const auto table : tables) {
		device->setTable(table.first, table.second);
	}

	return device;
}
