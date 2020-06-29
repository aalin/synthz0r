#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include <memory>
#include <limits>
#include <list>
#include <cmath>
#include "devices/base_device.hpp"
#include "audio/pulse_audio.hpp"
#include "audio/audio_buffer.hpp"
#include "utils.hpp"
#include "transport.hpp"
#include "channel.hpp"
#include "obj_ptr_index.hpp"

class Engine {
	public:
		Engine(unsigned int sampleRate, Audio::AudioBufferPtr buffer, std::list<Audio::AudioOutputPtr> audioOutputs)
		: _transport(sampleRate),
		  _audioOutputs(audioOutputs),
		  _buffer(buffer)
		{}

		void registerDevice(Devices::DevicePtr device) {
			_devices.add(device);
		}

		Devices::DevicePtr findDeviceById(uint32_t id) {
			return _devices.get(id);
		}

		ChannelPtr createChannel(std::string name) {
			ChannelPtr channel = std::make_shared<Channel>(name);
			_channels.push_back(channel);
			return channel;
		}

		bool removeChannel(uint32_t id) {
			const auto it = std::find_if(
				_channels.begin(),
				_channels.end(),
				[&id](auto channel) -> bool { return channel->id() == id; }
			);

			if (it == _channels.end()) {
				return false;
			}

			_channels.erase(it);

			return true;
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

		Sequences::SequencePtr insertSequence(uint32_t channelId, uint32_t start, uint32_t length) {
			ChannelPtr channel = getChannelById(channelId);

			if (channel == nullptr) {
				return nullptr;
			}

			Sequences::SequencePtr sequence = channel->insertSequence(start, length);
			_sequences.add(sequence);
			return sequence;
		}

		void addSequenceNote(uint32_t sequenceId, uint32_t start, uint32_t length, uint8_t note, uint8_t velocity) {
			Sequences::SequencePtr sequence = _sequences.get(sequenceId);

			if (sequence == nullptr) {
				return;
			}

			sequence->insertNote(start, length, note, velocity);
		}

		float update() {
			for (unsigned int i = 0; i < _buffer->size() / _buffer->numChannels(); i++) {
				StereoSample out;

				for (auto &channel : _channels) {
					out.add(channel->update(_transport, _noteEvents));
				}

				_buffer->set(i, out);

				_noteEvents.clear();
				_transport.update();
			}

			for (auto &output : _audioOutputs) {
				_buffer->write(output);
			}

			return _transport.secondsElapsedSinceStart();
		}

		void exit() {
			_hasRequestedExit = true;
		}

		bool hasRequestedExit() {
			return _hasRequestedExit;
		}

		Transport & transport() {
			return _transport;
		}

		void play() {
			_transport.play();
		}

		void pause() {
			_transport.pause();
			_noteEvents.push_back(NoteEvent::pauseAll());
		}

	private:
		Transport _transport;
		bool _hasRequestedExit = false;

		std::list<Audio::AudioOutputPtr> _audioOutputs;

		Audio::AudioBufferPtr _buffer;
		std::list<ChannelPtr> _channels;

		ObjPtrIndex<Devices::BaseDevice> _devices;
		ObjPtrIndex<Sequences::Sequence> _sequences;

		std::list<NoteEvent> _noteEvents;
};

#endif
