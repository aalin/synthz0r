#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include <memory>
#include <limits>
#include <list>
#include <cmath>
#include "devices/base_device.hpp"
#include "channel.hpp"
#include "pulse_audio.hpp"
#include "audio_buffer.hpp"
#include "utils.hpp"
#include "timer.hpp"

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

		ChannelPtr createChannel(std::string name) {
			ChannelPtr channel = std::make_shared<Channel>(name);
			_channels.push_back(channel);
			return channel;
		}

		void removeChannel(size_t) {
			throw std::runtime_error("TODO: Implement me");
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
};

#endif
