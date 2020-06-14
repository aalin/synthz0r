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
	static SampleFormat::Type type() { return Type::UINT8; }
};

template<>
struct fromType<int16_t> {
	static SampleFormat::Type type() { return Type::INT16; }
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

inline unsigned int bytesPerSample(SampleFormat::Type type) {
	switch (type) {
		case Type::UINT8:
			return 1;
		case Type::INT16:
			return 2;
		case Type::INT32:
			return 4;
		case Type::FLOAT32:
			return 4;
		default:
			return 0;
	}
}

inline unsigned int bitsPerSample(SampleFormat::Type type) {
	return bytesPerSample(type) * 8;
}
}

#endif
