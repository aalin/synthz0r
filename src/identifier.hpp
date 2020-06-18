#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <cstdint>

class Identifier {
	static uint32_t NEXT_ID;

	public:
		const uint32_t & get() const { return _id; }
		operator uint32_t() const { return _id; }

	private:
		uint32_t _id;
};

#endif
