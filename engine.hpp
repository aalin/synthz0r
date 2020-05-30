#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include <memory>
#include <limits>
#include <list>
#include "device.hpp"
#include "pulse_audio.hpp"

template<typename BufferType>
class Engine {
	public:
		Engine(const char *appName, unsigned int sampleRate, unsigned int numChannels, unsigned int bufferSize)
		: _running(false),
		  _tick(0),
		  _pa(PA::SampleFormat<BufferType>::format(), appName, sampleRate, numChannels),
		  _sampleRate(sampleRate),
		  _numChannels(numChannels),
		  _bufferSize(bufferSize)
		{
			_buffer = new BufferType[bufferSize];
		}

		~Engine() {
			_pa.drain();
			delete[] _buffer;
		}

		bool running() const {
			return _running;
		}

		void start() {
			_running = true;
		}

		std::shared_ptr<Device> addDevice(std::shared_ptr<Device> device) {
			_devices.push_back(device);
			return device;
		}

		float getTime() const {
			return  _tick / static_cast<float>(_sampleRate);
		}

		float update() {
			float amplitude = 0.2 * std::numeric_limits<BufferType>::max();

			for (unsigned int i = 0; i < _bufferSize / _numChannels; i++) {
				const float time = getTime();

				float v = 0.0;

				for (auto device : _devices) {
					v += device->update(time, _sampleRate);
				}

				for (unsigned int channel = 0; channel < _numChannels; channel++) {
					size_t idx = i * _numChannels + channel;

					if (channel == 1) {
						_buffer[idx] = 0;
						continue;
					}

					BufferType value = clip<BufferType>(v * amplitude);
					_buffer[idx] = value;
				}

				_tick++;
			}

			_pa.write(_buffer, sizeof(BufferType) * _bufferSize);

			return getTime();
		}

		void stop() {
			_running = false;
		}

	private:
		bool _running;
		unsigned int _tick;

		PulseAudio _pa;

		const unsigned int _sampleRate;
		const unsigned int _numChannels;
		const unsigned int _bufferSize;

		BufferType *_buffer;

		std::list<std::shared_ptr<Device> > _devices;

		template<typename T, typename X> T clip(X value) {
			constexpr T min = std::numeric_limits<T>::min();
			constexpr T max = std::numeric_limits<T>::max();

			if (value < min) {
				return min;
			}

			if (value > max) {
				return max;
			}

			return value;
		}
};

#endif
