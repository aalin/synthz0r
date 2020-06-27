#ifndef POSITION_HPP
#define POSITION_HPP

#include <ostream>

class Position {
	public:
		static constexpr unsigned int SIXTEENTH_LENGTH = 240;
		static constexpr unsigned int BEAT_LENGTH = SIXTEENTH_LENGTH * 4;
		static constexpr unsigned int BAR_LENGTH = BEAT_LENGTH * 4;

		static unsigned long ticksFromPosition(uint32_t bar, uint8_t beat, uint8_t sixteenths, uint8_t ticks) {
			return (
				bar * BAR_LENGTH +
				beat * BEAT_LENGTH +
				sixteenths * SIXTEENTH_LENGTH +
				ticks
			);
		}

		Position()
		: _totalTicks(0.0),
		  _ticksChanged(true)
		{}

		void update(const float &bpm, const float &sampleRate);

		uint32_t bar = 0;
		uint8_t beat = 0;
		uint8_t sixteenths = 0;
		uint8_t ticks = 0;

		void skipTo(double ticks) {
			_totalTicks = ticks;

			recalculate();
		}

		void skipTo(uint32_t bar, uint8_t beat, uint8_t sixteenths, uint8_t ticks) {
			_totalTicks = ticksFromPosition(bar, beat, sixteenths, ticks);

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

		bool ticksChanged() const {
			return _ticksChanged;
		}

	private:
		double _totalTicks;
		bool _ticksChanged;

		void recalculate();
};

std::ostream & operator<<(std::ostream &, const Position &);

#endif
