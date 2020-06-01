#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include "pa_sample_format.hpp"
#include "utils.hpp"

class AudioBufferBase {
	public:
		virtual pa_sample_format_t pulseAudioSampleFormat() const = 0;
		virtual void write(PulseAudio &pa) = 0;
		virtual size_t size() const = 0;
		virtual size_t numChannels() const = 0;
		virtual void set(size_t index, size_t channel, float value) = 0;
};

template<typename BufferType>
class AudioBuffer : public AudioBufferBase {
	public:
		AudioBuffer(size_t numChannels, size_t size)
		: _numChannels(numChannels),
		  _size(size)
		{
			_buffer = new BufferType[size];
		}

		~AudioBuffer() {
			delete[] _buffer;
		}

		pa_sample_format_t pulseAudioSampleFormat() const {
			return PA::SampleFormat<BufferType>::format();
		}

		void write(PulseAudio &pa) {
			pa.write(_buffer, sizeof(BufferType) * _size);
		}

		size_t size() const {
			return _size;
		}

		size_t numChannels() const {
			return _numChannels;
		}

		void set(size_t index, size_t channel, float value) {
			constexpr float amplitude = std::numeric_limits<BufferType>::max();
			size_t idx = index * _numChannels + channel;
			_buffer[idx] = Utils::clip<BufferType>(value * amplitude);
		}

	private:
		const size_t _numChannels;
		const size_t _size;
		BufferType *_buffer;
};

typedef AudioBuffer<uint8_t> AudioBuffer8Bit;
typedef AudioBuffer<int16_t> AudioBuffer16Bit;
typedef AudioBuffer<int32_t> AudioBuffer32Bit;
typedef AudioBuffer<float> AudioBufferFloat;

#endif
