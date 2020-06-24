#ifndef POSITION_HPP
#define POSITION_HPP

#include <ostream>

class Position {
	public:
		void update(const float &bpm, const float &sampleRate);

		uint32_t bar = 0;
		uint8_t beat = 0;
		uint8_t sixteenths = 0;
		uint8_t ticks = 0;

		void skipTo(double ticks) {
			_totalTicks = ticks;

			recalculate();
		}

		void skipTo(uint32_t _bar, uint32_t _beat, uint32_t _sixteenths, uint32_t _ticks) {
			_totalTicks =
				_ticks +
				_sixteenths * 240 +
				_beat * 240 * 4 +
				_bar * 240 * 4 * 4;

			recalculate();
		}

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

		void recalculate();
};

std::ostream & operator<<(std::ostream &, const Position &);

#endif
