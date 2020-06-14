#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include "utils.hpp"
#include "stereo_sample.hpp"
#include "sample_format.hpp"
#include "audio_output.hpp"

class AudioBufferBase {
	public:
		virtual SampleFormat::Type sampleFormat() const = 0;
		virtual void write(AudioOutputPtr) = 0;
		virtual size_t size() const = 0;
		virtual size_t numChannels() const = 0;
		virtual void set(size_t index, size_t channel, float value) = 0;
		virtual void set(size_t index, const StereoSample &sample) = 0;
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

		SampleFormat::Type sampleFormat() const {
			return SampleFormat::fromType<BufferType>::type();
		}

		void write(AudioOutputPtr output) {
			output->write(_buffer, sizeof(BufferType) * _size);
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

		void set(size_t index, const StereoSample &sample) {
			set(index, 0, sample.left);
			set(index, 1, sample.right);
		}

	private:
		const size_t _numChannels;
		const size_t _size;
		BufferType *_buffer;
};

typedef std::shared_ptr<AudioBufferBase> AudioBufferPtr;
typedef AudioBuffer<uint8_t> AudioBuffer8Bit;
typedef AudioBuffer<int16_t> AudioBuffer16Bit;
typedef AudioBuffer<int32_t> AudioBuffer32Bit;
typedef AudioBuffer<float> AudioBufferFloat;

#endif
