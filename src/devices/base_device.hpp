#ifndef DEVICES__BASE_DEVICE_HPP
#define DEVICES__BASE_DEVICE_HPP

#include <memory>
#include "base_device/t_outputs.hpp"
#include "base_device/parameter_list.hpp"
#include "../timer.hpp"
#include "../stereo_sample.hpp"

namespace Devices {
class BaseDevice;

typedef std::shared_ptr<BaseDevice> DevicePtr;

class BaseDevice {
	public:
		typedef TOutputs<DevicePtr> Outputs;

		BaseDevice(std::string name, std::initializer_list<Parameter> vars = {})
		: _name(name),
		  _parameters(vars)
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

		const ParameterList & parameters() const {
			return _parameters;
		}

		const int & getParam(const std::string &var) const {
			return _parameters.get(var).value();
		}

		void setParam(const std::string &var, int value) {
			_parameters.get(var).setValue(value);
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
		ParameterList _parameters;
};
};

#endif
