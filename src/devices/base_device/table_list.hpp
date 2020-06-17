#ifndef DEVICES__BASE_DEVICE__TABLE_LIST_HPP
#define DEVICES__BASE_DEVICE__TABLE_LIST_HPP

#include <string>
#include <map>
#include <initializer_list>
#include "table.hpp"
#include "../../utils.hpp"
#include "../../performance_log.hpp"

namespace Devices {
	class TableList {
		typedef std::map<std::string, Table> TableMap;

		struct value_iterator : public TableMap::const_iterator {
			value_iterator(TableMap::const_iterator it)
			: TableMap::const_iterator(std::move(it))
			{}

			const Table & operator*() const {
				return TableMap::const_iterator::operator*().second;
			}
		};

		public:
			TableList(std::initializer_list<Table> tables) {
				for (const Table &table : tables) {
					_tables.insert({table.name(), table});
				}
			}

			value_iterator begin() const {
				return value_iterator(_tables.begin());
			}

			value_iterator end() const {
				return value_iterator(_tables.end());
			}

			const Table & get(std::string name) const {
				auto var = _tables.find(name);

				if (var == _tables.end()) {
					std::cerr << "Tried to get table" << name << std::endl;
					throw std::runtime_error("Unknown table name: " + name);
				}

				return var->second;
			}

			Table & get(std::string name) {
				auto table = _tables.find(name);

				if (table == _tables.end()) {
					std::cerr << "Tried to get table" << name << std::endl;
					throw std::runtime_error("Unknown table name: " + name);
				}

				return table->second;
			}

		private:
			TableMap _tables;
	};
}

#endif
