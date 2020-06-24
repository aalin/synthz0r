#ifndef TRACK_HPP
#define TRACK_HPP

#include "sequence.hpp"

class Track {
	public:
		Track() {
		}

		void insertSequence(uint64_t start, uint64_t length);

		bool eraseSequence(uint64_t position);

	private:
		std::list<Sequence> _sequences;
};

#endif
