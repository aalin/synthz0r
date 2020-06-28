#include "position.hpp"
#include <iostream>
#include <regex>

uint32_t Position::parse(const std::string &str) {
	const std::regex re("^(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)$");
	std::smatch match;

	if (!std::regex_match(str, match, re)) {
		throw std::runtime_error("Could not parse string");
	}

	const uint32_t bar = atoi(match.str(1).c_str());
	const uint8_t beat = atoi(match.str(2).c_str());
	const uint8_t sixteenths = atoi(match.str(3).c_str());
	const uint8_t ticks = atoi(match.str(4).c_str());

	return ticksFromPosition(bar, beat, sixteenths, ticks);
}

std::string Position::toString() const {
	std::ostringstream oss;

	oss <<
		"Position(" <<
		(uint32_t)bar << "." <<
		(uint32_t)beat << "." <<
		(uint32_t)sixteenths << "." <<
		(uint32_t)ticks << ")";

	return oss.str();
}

std::ostream & operator<<(std::ostream &out, const Position &position) {
	return out << position.toString();
}

void Position::update(const float &bpm, const float &sampleRate) {
	const double ticksPerSecond = (bpm * 4.0 * 240.0) / 60.0;

	recalculate(_totalTicks + ticksPerSecond / sampleRate);
}

void Position::recalculate(double newTicks) {
	uint32_t previousTicks = _totalTicks;

	_totalTicks = newTicks;

	uint32_t tmp = newTicks;

	ticks = tmp % 240;
	tmp /= 240;
	sixteenths = tmp % 4;
	tmp /= 4;
	beat = tmp % 4;
	bar = tmp / 4;

	_ticksChanged = static_cast<uint32_t>(_totalTicks) != previousTicks;
}
