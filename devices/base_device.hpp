#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <memory>
#include "base_device/t_outputs.hpp"
#include "base_device/variable_list.hpp"
#include "../timer.hpp"
#include "../stereo_sample.hpp"

namespace Devices {
class BaseDevice;

typedef std::shared_ptr<BaseDevice> DevicePtr;

class BaseDevice {
	public:
		typedef TOutputs<DevicePtr> Outputs;

		BaseDevice(std::string name, std::initializer_list<Variable> vars = {})
		: _name(name),
		  _variables(vars)
		{ }

		virtual void update(const Timer &, float) { }
		virtual void update(const Timer &) { }

		std::string name() const { return _name; }
		void setName(std::string name) { _name = name; }

		Outputs & outputs() {
			return _outputs;
		}

		virtual void input(const Timer &, const float &) {
			throw "BaseDevice received input";
		}

		virtual void input(const Timer &, const StereoSample &) {
			throw "BaseDevice received input";
		}

		const VariableList & variables() const {
			return _variables;
		}

		const int & getValue(const std::string &var) const {
			return _variables.get(var).value();
		}

		void setValue(const std::string &var, int &value) {
			_variables.get(var).setValue(value);
		}

	protected:
		void output(const Timer &timer, const float &value) {
			_outputs.output(timer, value);
		}

		void output(const Timer &timer, const StereoSample &value) {
			_outputs.output(timer, value);
		}

	private:
		std::string _name;
		Outputs _outputs;
		VariableList _variables;
};
};

#endif
