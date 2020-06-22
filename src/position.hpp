#ifndef POSITION_HPP
#define POSITION_HPP

#include <ostream>

class Position {
	public:
		void update(const float &bpm, const float &sampleRate) {
			const double ticksPerSecond = (bpm * 4.0 * 240.0) / 60.0;
			_totalTicks += ticksPerSecond / sampleRate;

			uint32_t tmp = _totalTicks;
			ticks = tmp % 240;
			tmp /= 240;
			sixteenths = tmp % 4;
			tmp /= 4;
			beat = tmp % 4;
			bar = tmp / 4;
		}

		uint32_t bar = 0;
		uint8_t beat = 0;
		uint8_t sixteenths = 0;
		uint8_t ticks = 0;

		float total4ths() const {
			return _totalTicks / 960.0;
		}

		float total8ths() const {
			return _totalTicks / 480.0;
		}

		float total16ths() const {
			return _totalTicks / 240.0;
		}

		float total32ths() const {
			return _totalTicks / 120.0;
		}

		float total64ths() const {
			return _totalTicks / 60.0;
		}

		float total128ths() const {
			return _totalTicks / 30.0;
		}

		float total256ths() const {
			return _totalTicks / 15.0;
		}

		double totalTicks() const {
			return _totalTicks;
		}

	private:
		double _totalTicks;
};

std::ostream & operator<<(std::ostream &, const Position &);

#endif
