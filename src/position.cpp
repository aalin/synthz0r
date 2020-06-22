#include "position.hpp"

std::ostream & operator<<(std::ostream &out, const Position &position) {
	return out <<
		"Position(" <<
		(int)position.bar << "." <<
		(int)position.beat << "." <<
		(int)position.sixteenths << "." <<
		(int)position.ticks << ")";
}
