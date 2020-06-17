#ifndef DEVICE_FACTORY_HPP
#define DEVICE_FACTORY_HPP

#include <list>
#include <string>
#include "devices/base_device.hpp"

namespace DeviceFactory {
	Devices::DevicePtr create(std::string name);

	const std::list<std::string> getDeviceNames();
}

#endif
