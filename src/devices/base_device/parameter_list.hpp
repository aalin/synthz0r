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
			ParameterList() {
			}

			ParameterList(std::initializer_list<Parameter> params) {
				for (Parameter param : params) {
					_parameters.insert({param.name(), param});
				}
			}

			value_iterator begin() const {
				return value_iterator(_parameters.begin());
			}

			value_iterator end() const {
				return value_iterator(_parameters.end());
			}

			const Parameter & setParam(const std::string &name, const int32_t &value) {
				Parameter &param = get(name);
				param.setValue(value);
				return param;
			}

			const Parameter & get(const std::string &name) const {
				auto param = _parameters.find(name);

				if (param == _parameters.end()) {
					std::cerr << "Tried to get parameter" << name << std::endl;
					throw std::runtime_error("Unknown parameter name");
				}

				return param->second;
			}

			Parameter & get(const std::string &name) {
				auto param = _parameters.find(name);

				if (param == _parameters.end()) {
					std::cerr << "Tried to get parameter" << name << std::endl;
					throw std::runtime_error("Unknown parameter name");
				}

				return param->second;
			}

		private:
			ParameterMap _parameters;
	};

	std::ostream & operator<<(std::ostream &out, const Devices::Parameter &p);
}

#endif
