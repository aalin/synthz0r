#ifndef DEVICES__BASE_DEVICE__VARIABLE_LIST_HPP
#define DEVICES__BASE_DEVICE__VARIABLE_LIST_HPP

#include <string>
#include <map>
#include "../../utils.hpp"

namespace Devices {
class Variable {
	public:
		Variable(std::string name, int minValue, int maxValue, int defaultValue, int &value)
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

		const Variable & setValue(int newValue) {
			_value = Utils::clamp(newValue, _min, _max);
			return *this;
		}

	private:
		const std::string _name;
		const int _min;
		const int _max;
		const int _defaultValue;
		int &_value;
};

class VariableList {
	public:
		VariableList(std::initializer_list<Variable> vars) {
			for (Variable var : vars) {
				std::cout << "Inserting variable " << var.name() << std::endl;
				_variables.insert({var.name(), var});
			}
		}

		const Variable & get(std::string name) const {
			auto var = _variables.find(name);

			if (var == _variables.end()) {
				std::cerr << "Tried to set variable" << name << std::endl;
				throw "Unknown variable name";
			}

			return var->second;
		}

		Variable & get(std::string name) {
			auto var = _variables.find(name);

			if (var == _variables.end()) {
				std::cerr << "Tried to set variable" << name << std::endl;
				throw "Unknown variable name";
			}

			return var->second;
		}

	private:
		std::map<std::string, Variable> _variables;
};
};

#endif
