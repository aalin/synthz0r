#include <iostream>
#include "parameter.hpp"

std::ostream & Devices::operator<<(std::ostream &out, const Devices::Parameter &p) {
	out <<
		"Parameter(" << p.name() <<
		" [" << p.min() << ".." << p.max() << "] " <<
		" " << p.value() << ")";
	return out;
}
