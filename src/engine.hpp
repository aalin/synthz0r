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
#include "channel.hpp"

class Engine {
	public:
		Engine(unsigned int sampleRate, AudioBufferPtr buffer, AudioOutputPtr audioOutput)
		: _timer(sampleRate),
		  _audioOutput(audioOutput),
		  _sampleRate(sampleRate),
		  _buffer(buffer)
		{ }

		~Engine() {
			_audioOutput->drain();
		}

		const Timer & timer() const {
			return _timer;
		}

		void registerDevice(Devices::DevicePtr device) {
			_devices[device->id()] = device;
			cleanExpiredDevices();
		}

		Devices::DevicePtr findDeviceById(uint32_t id) {
			cleanExpiredDevices();

			auto it = _devices.find(id);

			if (it == _devices.end()) {
				return nullptr;
			}

			return it->second.lock();
		}

		ChannelPtr createChannel(std::string name) {
			ChannelPtr channel = std::make_shared<Channel>(name);
			_channels.push_back(channel);
			return channel;
		}

		bool removeChannel(uint32_t id) {
			for (auto it = _channels.begin(); it != _channels.end(); it++) {
				if ((*it)->id() == id) {
					_channels.erase(it);
					return true;
				}
			}

			return false;
		}

		ChannelPtr getChannelById(uint32_t id) {
			for (auto channel : _channels) {
				if (channel->id() == id) {
					return channel;
				}
			}

			return nullptr;
		}

		const std::list<ChannelPtr> & channels() const {
			return _channels;
		}

		float update() {
			for (unsigned int i = 0; i < _buffer->size() / _buffer->numChannels(); i++) {
				StereoSample out;

				for (auto &channel : _channels) {
					out.add(channel->update(_timer, {}));
				}

				_buffer->set(i, out);

				_timer.update();
			}

			_buffer->write(_audioOutput);

			return _timer.seconds();
		}

	private:
		Timer _timer;

		AudioOutputPtr _audioOutput;

		const unsigned int _sampleRate;

		AudioBufferPtr _buffer;
		std::list<ChannelPtr> _channels;
		std::map<uint32_t, std::weak_ptr<Devices::BaseDevice> > _devices;

		void cleanExpiredDevices() {
			auto it = _devices.begin();

			while (it != _devices.end()) {
				if (it->second.expired()) {
					it = _devices.erase(it);
				} else {
					it++;
				}
			}
		}
};

#endif
