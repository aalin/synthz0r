#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include <memory>
#include <limits>
#include <list>
#include <cmath>
#include "device.hpp"
#include "pulse_audio.hpp"
#include "audio_buffer.hpp"
#include "utils.hpp"

class Engine {
	public:
		Engine(const char *appName, unsigned int sampleRate, std::shared_ptr<AudioBufferBase> buffer)
		: _running(false),
		  _tick(0),
		  _pa(buffer->pulseAudioSampleFormat(), appName, sampleRate, buffer->numChannels()),
		  _sampleRate(sampleRate)
		{
			_buffer = buffer;
		}

		~Engine() {
			_pa.drain();
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

		float getTime() {
			return _tick;
		}

		float getScaledTime() const {
			return  _tick / static_cast<float>(_sampleRate);
		}

		float update() {
			const float time = getScaledTime();

			for (auto device : _devices) {
				device->tick(time, _sampleRate);
			}

			for (unsigned int i = 0; i < _buffer->size() / _buffer->numChannels(); i++) {
				float v = 0.0;

				for (auto device : _devices) {
					v += device->update(time, _sampleRate);
				}

				for (unsigned int channel = 0; channel < _buffer->numChannels(); channel++) {
					if (channel == 1) {
						_buffer->set(i, channel, 0.0);
						continue;
					}

					_buffer->set(i, channel, v);
				}

				_tick++;
			}

			_buffer->write(_pa);

			return getScaledTime();
		}

		void stop() {
			_running = false;
		}

	private:
		bool _running;
		unsigned int _tick;

		PulseAudio _pa;

		const unsigned int _sampleRate;

		std::shared_ptr<AudioBufferBase> _buffer;

		std::list<std::shared_ptr<Device> > _devices;
};

#endif
