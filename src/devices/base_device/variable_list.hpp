#ifndef DEVICES__BASE_DEVICE__VARIABLE_LIST_HPP
#define DEVICES__BASE_DEVICE__VARIABLE_LIST_HPP

#include <string>
#include <map>
#include "../../utils.hpp"
#include "../../performance_log.hpp"

namespace Devices {
class Parameter {
	public:
		Parameter(std::string name, int minValue, int maxValue, int defaultValue, int &value)
		: _name(name),
		  _min(minValue),
		  _max(maxValue),
		  _defaultValue(defaultValue),
		  _value(value)
		{
			_value = defaultValue;
		}

		const std::string & name() const {
			return _name;
		}

		const int & defaultValue() const {
			return _defaultValue;
		}

		const int & min() const {
			return _min;
		}

		const int & max() const {
			return _max;
		}

		const int & value() const {
			return _value;
		}

		const Parameter & setValue(int newValue) {
			_value = Utils::clamp(newValue, _min, _max);
			std::cout << "Setting " << _name << " to " << _value << std::endl;
			return *this;
		}

	private:
		const std::string _name;
		const int _min;
		const int _max;
		const int _defaultValue;
		int &_value;
};

std::ostream & operator<<(std::ostream &out, const Parameter &p) {
	out <<
		"Parameter(" << p.name() <<
		" [" << p.min() << ".." << p.max() << "] " <<
		" " << p.value() << ")";
	return out;
}

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
};

#endif
