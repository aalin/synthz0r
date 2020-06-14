#ifndef DEVICES__BASE_DEVICE__PARAMETER_LIST_HPP
#define DEVICES__BASE_DEVICE__PARAMETER_LIST_HPP

#include <string>
#include <map>
#include <initializer_list>
#include "parameter.hpp"
#include "../../utils.hpp"
#include "../../performance_log.hpp"

namespace Devices {
class ParameterList {
	typedef std::map<std::string, Parameter> ParameterMap;

	struct value_iterator : public ParameterMap::const_iterator {
		value_iterator(ParameterMap::const_iterator it)
		: ParameterMap::const_iterator(std::move(it))
		{}

		const Parameter & operator*() const {
			return ParameterMap::const_iterator::operator*().second;
		}
	};

	public:
		ParameterList(std::initializer_list<Parameter> vars) {
			PerformanceLog perf;

			for (Parameter var : vars) {
				_parameters.insert({var.name(), var});
			}

			perf.log("Creating parameters");
		}

		value_iterator begin() const {
			return value_iterator(_parameters.begin());
		}

		value_iterator end() const {
			return value_iterator(_parameters.end());
		}

		const Parameter & get(std::string name) const {
			auto var = _parameters.find(name);

			if (var == _parameters.end()) {
				std::cerr << "Tried to get parameter" << name << std::endl;
				throw "Unknown parameter name";
			}

			return var->second;
		}

		Parameter & get(std::string name) {
			auto var = _parameters.find(name);

			if (var == _parameters.end()) {
				std::cerr << "Tried to get parameter" << name << std::endl;
				throw "Unknown parameter name";
			}

			return var->second;
		}

	private:
		ParameterMap _parameters;
};

std::ostream & operator<<(std::ostream &out, const Devices::Parameter &p);
};

#endif
