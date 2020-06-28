#ifndef SEQUENCES__TRACK_HPP
#define SEQUENCES__TRACK_HPP

#include "sequence.hpp"

namespace Sequences {
	class Track {
		public:
			SequencePtr insertSequence(uint32_t start, uint32_t length);

			bool eraseSequence(uint32_t position);

			void getEventsAt(std::list<NoteEvent> &events, uint32_t position) const;

		private:
			std::list<SequencePtr> _sequences;
	};
}

#endif
