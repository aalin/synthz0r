#ifndef DEVICES__BASE_DEVICE__PARAMETER_HPP
#define DEVICES__BASE_DEVICE__PARAMETER_HPP

#include "../../utils.hpp"

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
			// std::cout << "Setting " << _name << " to " << _value << std::endl;
			return *this;
		}

	private:
		const std::string _name;
		const int _min;
		const int _max;
		const int _defaultValue;
		int &_value;
};
}

#endif
