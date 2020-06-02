#ifndef SAMPLE_FORMAT_HPP
#define SAMPLE_FORMAT_HPP

namespace SampleFormat {
enum Type {
	UINT8,
	INT16,
	INT32,
	FLOAT32,
	INVALID
};

template<typename T>
struct fromType {
	static SampleFormat::Type type() {
		return Type::INVALID;
	}
};

template<>
struct fromType<uint8_t> {
	static SampleFormat::Type type() {
		return Type::UINT8;
	}
};

template<>
struct fromType<int16_t> {
	static SampleFormat::Type type() {
		return Type::INT16;
	}
};

template<>
struct fromType<int32_t> {
	static SampleFormat::Type type() {
		return Type::INT32;
	}
};

template<>
struct fromType<float> {
	static SampleFormat::Type type() {
		return Type::FLOAT32;
	}
};
};

#endif
