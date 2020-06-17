#include <iostream>
#include "table.hpp"

std::ostream & Devices::operator<<(std::ostream &out, const Devices::Table &p) {
	out << "Table(" << p.name() << ")";
	// TODO: Print all values
	return out;
}
