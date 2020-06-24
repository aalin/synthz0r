#ifndef DEVICES__BASE_DEVICE_HPP
#define DEVICES__BASE_DEVICE_HPP

#include <memory>
#include "base_device/parameter_list.hpp"
#include "base_device/table_list.hpp"
#include "../transport.hpp"
#include "../stereo_sample.hpp"
#include "../note_event.hpp"
#include "../identifier.hpp"

namespace Devices {
	class BaseDevice {
		public:
			BaseDevice(std::string name)
			: _name(name),
			  _respondingToParameterChange(false)
			{}

			virtual ~BaseDevice() {}

			uint32_t id() const { return _id; }

			std::string name() const { return _name; }
			void setName(std::string name) { _name = name; }

			const ParameterList & parameters() const {
				return _parameters;
			}

			const int & getParam(const std::string &name) const {
				return _parameters.get(name).value();
			}

			void setParam(const std::string &name, const int32_t &value) {
				if (_respondingToParameterChange) {
					std::cerr << "Error: Can not set parameters from respondToParameterChange" << std::endl;
					return;
				}

				const auto &param = _parameters.setParam(name, value);

				_respondingToParameterChange = true;
				respondToParameterChange(param);
				_respondingToParameterChange = false;
			}

			const TableList & tables() const {
				return _tables;
			}

			void setTable(const std::string &name, const std::vector<int> &data) {
				_tables.setTable(name, data);
			}

			const Table & getTable(const std::string &name) const {
				return _tables.get(name);
			}

		protected:
			void setupParameters(std::initializer_list<Parameter> params) {
				_parameters = params;
			}

			void setupTables(std::initializer_list<Table> tables) {
				_tables = tables;
			}

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
