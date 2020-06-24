#include "position.hpp"
#include <iostream>

std::ostream & operator<<(std::ostream &out, const Position &position) {
	return out <<
		"Position(" <<
		(int)position.bar << "." <<
		(int)position.beat << "." <<
		(int)position.sixteenths << "." <<
		(int)position.ticks << ")";
}

void Position::update(const float &bpm, const float &sampleRate) {
	const double ticksPerSecond = (bpm * 4.0 * 240.0) / 60.0;
	_totalTicks += ticksPerSecond / sampleRate;

	recalculate();
}

void Position::recalculate() {
	uint32_t tmp = _totalTicks;
	ticks = tmp % 240;
	tmp /= 240;
	sixteenths = tmp % 4;
	tmp /= 4;
	beat = tmp % 4;
	bar = tmp / 4;
}
