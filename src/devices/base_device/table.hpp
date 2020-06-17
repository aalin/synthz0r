#ifndef DEVICES__BASE_DEVICE__TABLE_HPP
#define DEVICES__BASE_DEVICE__TABLE_HPP

#include <vector>
#include "../../utils.hpp"

namespace Devices {
	class Table {
		public:
			typedef std::vector<int> TableType;

			Table(std::string name, int minValue, int maxValue, int defaultValue, size_t initialSize, TableType &data)
			: _name(name),
			  _min(minValue),
			  _max(maxValue),
			  _defaultValue(defaultValue),
			  _data(data)
			{
				_data.resize(initialSize, defaultValue);
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

			const TableType & data() const {
				return _data;
			}

			void setData(const TableType &data) {
				_data.assign(data.begin(), data.end());
				clampData();
			}

		private:
			const std::string _name;
			const int _min;
			const int _max;
			const int _defaultValue;
			TableType &_data;

			void clampData() {
				std::transform(
					_data.begin(),
					_data.end(),
					_data.begin(),
					[&](int v) -> int { return Utils::clamp(v, _min, _max); }
				);
			}
	};

	std::ostream & operator<<(std::ostream &out, const Devices::Table &p);
}

#endif

