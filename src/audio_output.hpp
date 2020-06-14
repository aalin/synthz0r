#ifndef AUDIO_OUTPUT_HPP
#define AUDIO_OUTPUT_HPP

#include <memory>

class AudioOutput {
	public:
		virtual void write(uint8_t buf[], unsigned int length) = 0;
		virtual void write(int16_t buf[], unsigned int length) = 0;
		virtual void write(int32_t buf[], unsigned int length) = 0;
		virtual void write(float buf[], unsigned int length) = 0;

		virtual void drain() = 0;
};

typedef std::shared_ptr<AudioOutput> AudioOutputPtr;

#endif
