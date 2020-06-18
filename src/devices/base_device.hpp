#ifndef DEVICES__BASE_DEVICE_HPP
#define DEVICES__BASE_DEVICE_HPP

#include <memory>
#include "base_device/t_outputs.hpp"
#include "base_device/parameter_list.hpp"
#include "base_device/table_list.hpp"
#include "../timer.hpp"
#include "../stereo_sample.hpp"
#include "../note_event.hpp"

namespace Devices {
class BaseDevice;

typedef std::shared_ptr<BaseDevice> DevicePtr;

class BaseDevice {
	public:
		typedef TOutputs<DevicePtr> Outputs;

		BaseDevice(
			std::string name,
			std::initializer_list<Parameter> params = {},
			std::initializer_list<Table> tables = {}
		)
		: _name(name),
		  _parameters(params),
		  _tables(tables),
		  _respondingToParameterChange(false)
		{ }

		virtual ~BaseDevice() {}

		uintptr_t id() const {
			return reinterpret_cast<uintptr_t>(this);
		}

		virtual void update(const Timer &, float) { }
		virtual void update(const Timer &) { }

		std::string name() const { return _name; }
		void setName(std::string name) { _name = name; }

		Outputs & outputs() {
			return _outputs;
		}

		virtual void input(const Timer &, const float &) {
			std::cerr << name() << " should implement input(const Timer &, const float &)" << std::endl;
			// throw "BaseDevice received input";
		}

		virtual void input(const Timer &, const StereoSample &) {
			std::cerr << name() << " should implement input(const Timer &, const StereoSample &)" << std::endl;
			// throw "BaseDevice received input";
		}

		virtual void input(const Timer &, const NoteEvent &) {
			std::cerr << name() << " should implement input(const Timer &, const NoteEvent &)" << std::endl;
			// throw "BaseDevice received input";
		}

		const ParameterList & parameters() const {
			return _parameters;
		}

		const int & getParam(const std::string &name) const {
			return _parameters.get(name).value();
		}

		void setParam(const std::string &name, int value) {
			if (_respondingToParameterChange) {
				std::cerr << "Error: Can not set parameters from respondToParameterChange" << std::endl;
				return;
			}

			Parameter &param = _parameters.get(name);
			param.setValue(value);

			_respondingToParameterChange = true;
			respondToParameterChange(param);
			_respondingToParameterChange = false;
		}

		void setTable(const std::string &name, std::vector<int> data) {
			Table &table = _tables.get(name);
			table.setData(data);
		}

	protected:
		void output(const Timer &timer, const float &value) {
			_outputs.output(timer, value);
		}

		void output(const Timer &timer, const StereoSample &value) {
			_outputs.output(timer, value);
		}

		void output(const Timer &timer, const NoteEvent &value) {
			_outputs.output(timer, value);
		}

		virtual void respondToParameterChange(const Parameter &) {
			// Implement me to handle parameter updates
		}

	private:
		std::string _name;
		Outputs _outputs;
		ParameterList _parameters;
		TableList _tables;

		bool _respondingToParameterChange;
};
}

#endif
