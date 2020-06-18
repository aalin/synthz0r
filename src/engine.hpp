#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include <memory>
#include <limits>
#include <list>
#include <cmath>
#include "devices/base_device.hpp"
#include "devices/output_device.hpp"
#include "pulse_audio.hpp"
#include "audio_buffer.hpp"
#include "utils.hpp"
#include "timer.hpp"

class Engine {
	public:
		typedef std::list<Devices::DevicePtr> DeviceList;

		Engine(unsigned int sampleRate, AudioBufferPtr buffer, AudioOutputPtr audioOutput)
		: _timer(sampleRate),
		  _audioOutput(audioOutput),
		  _sampleRate(sampleRate),
		  _outputDevice(std::make_shared<Devices::OutputDevice>()),
		  _buffer(buffer)
		{ }

		~Engine() {
			_audioOutput->drain();
		}

		const Timer & timer() const {
			return _timer;
		}

		Devices::DevicePtr addDevice(Devices::DevicePtr device) {
			_devices.push_back(device);
			return device;
		}

		std::shared_ptr<Devices::OutputDevice> getOutputDevice() {
			return _outputDevice;
		}

		float update() {
			for (unsigned int i = 0; i < _buffer->size() / _buffer->numChannels(); i++) {
				for (auto device : _devices) {
					device->update(_timer, 0.0);
				}

				const StereoSample &sample = _outputDevice->getSample();
				_buffer->set(i, sample);

				_outputDevice->reset();
				_timer.update();
			}

			_buffer->write(_audioOutput);

			return _timer.seconds();
		}

		const DeviceList & devices() const {
			return _devices;
		}

		Devices::DevicePtr findDeviceById(uint64_t id) {
			if (id == 0) {
				return _outputDevice;
			}

			auto it = std::find_if(
				_devices.begin(),
				_devices.end(),
				[&id](Devices::DevicePtr device) -> bool { return device->id() == id; }
			);

			if (it == _devices.end()) {
				return nullptr;
			}

			return *it;
		}

	private:
		Timer _timer;

		AudioOutputPtr _audioOutput;

		const unsigned int _sampleRate;

		std::shared_ptr<Devices::OutputDevice> _outputDevice;

		AudioBufferPtr _buffer;
		DeviceList _devices;
};

#endif
