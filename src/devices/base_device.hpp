#ifndef DEVICES__BASE_DEVICE_HPP
#define DEVICES__BASE_DEVICE_HPP

#include <memory>
#include "base_device/parameter_list.hpp"
#include "base_device/table_list.hpp"
#include "../timer.hpp"
#include "../stereo_sample.hpp"
#include "../note_event.hpp"
#include "../identifier.hpp"

namespace Devices {
	class BaseDevice {
		public:
			BaseDevice(
				std::string name,
				std::initializer_list<Parameter> params = {},
				std::initializer_list<Table> tables = {}
			)
			: _name(name),
			  _parameters(params),
			  _tables(tables),
			  _respondingToParameterChange(false)
			{}

			virtual ~BaseDevice() {}

			uint32_t id() const { return _id; }

			virtual void update(const Timer &, float) { }
			virtual void update(const Timer &) { }

			std::string name() const { return _name; }
			void setName(std::string name) { _name = name; }

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

			const TableList & tables() const {
				return _tables;
			}

			void setTable(const std::string &name, std::vector<int> data) {
				Table &table = _tables.get(name);
				table.setData(data);
			}

		protected:
			virtual void respondToParameterChange(const Parameter &) {
				// Implement me to handle parameter updates
			}

		private:
			Identifier _id;
			std::string _name;
			ParameterList _parameters;
			TableList _tables;

			bool _respondingToParameterChange;
	};

	typedef std::shared_ptr<BaseDevice> DevicePtr;
}

#endif
