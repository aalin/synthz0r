#include <iostream>
#include "variable_list.hpp"

std::ostream & operator<<(std::ostream &out, const Devices::Parameter &p) {
	out <<
		"Parameter(" << p.name() <<
		" [" << p.min() << ".." << p.max() << "] " <<
		" " << p.value() << ")";
	return out;
}
