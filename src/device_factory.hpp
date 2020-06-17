#ifndef DEVICE_FACTORY_HPP
#define DEVICE_FACTORY_HPP

#include <list>
#include <string>
#include "devices/base_device.hpp"

namespace DeviceFactory {
	typedef std::initializer_list<std::pair<std::string, int> > ParameterInitializerList;
	typedef std::initializer_list<std::pair<std::string, std::vector<int> > > TableInitializerList;

	Devices::DevicePtr create(std::string name);
	Devices::DevicePtr create(std::string name, ParameterInitializerList);
	Devices::DevicePtr create(std::string name, ParameterInitializerList, TableInitializerList);

	const std::list<std::string> getDeviceNames();
}

#endif
