#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include <memory>
#include <limits>
#include <list>
#include <cmath>
#include "devices/base_device.hpp"
#include "pulse_audio.hpp"
#include "audio_buffer.hpp"
#include "utils.hpp"
#include "timer.hpp"

class Engine {
	public:
		Engine(const char *appName, unsigned int sampleRate, std::shared_ptr<AudioBufferBase> buffer)
		: _running(false),
		  _timer(sampleRate),
		  _pa(buffer->pulseAudioSampleFormat(), appName, sampleRate, buffer->numChannels()),
		  _sampleRate(sampleRate)
		{
			_buffer = buffer;
		}

		~Engine() {
			_pa.drain();
		}

		const Timer & timer() const {
			return _timer;
		}

		bool running() const {
			return _running;
		}

		void start() {
			_running = true;
		}

		Devices::DevicePtr addDevice(Devices::DevicePtr device) {
			_devices.push_back(device);
			return device;
		}

		float update() {
			const float seconds = _timer.getSeconds();

			for (auto device : _devices) {
				device->tick(_timer);
			}

			for (unsigned int i = 0; i < _buffer->size() / _buffer->numChannels(); i++) {
				float v = 0.0;

				for (auto device : _devices) {
					v += device->update(_timer);
				}

				for (unsigned int channel = 0; channel < _buffer->numChannels(); channel++) {
					if (channel == 1) {
						_buffer->set(i, channel, 0.0);
						continue;
					}

					_buffer->set(i, channel, v);
				}

				_timer.tick();
			}

			_buffer->write(_pa);

			return _timer.getSeconds();
		}

		void stop() {
			_running = false;
		}

	private:
		bool _running;
		Timer _timer;

		PulseAudio _pa;

		const unsigned int _sampleRate;

		std::shared_ptr<AudioBufferBase> _buffer;

		std::list<Devices::DevicePtr> _devices;
};

#endif
