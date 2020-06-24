#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include "devices.hpp"
#include "track.hpp"
#include "channel/device_container.hpp"

class Channel {
	public:
		Channel(std::string name)
		: _name(name)
		{
			std::cout << "Creating channel: " << _name << " id: " << _id.get() << std::endl;
		}

		~Channel() {
			std::cout << "Destroying channel: " << _name << " id: " << _id.get() << std::endl;
		}

		uint32_t id() const { return _id; }

		const std::string & name() {
			return _name;
		}

		void insertNoteDevice(size_t index, Devices::NoteDevicePtr noteDevice) {
			_noteDevices.insertAt(index, noteDevice);
		}

		void appendNoteDevice(Devices::NoteDevicePtr noteDevices) {
			_noteDevices.append(noteDevices);
		}

		void removeNoteDevice(size_t index) {
			_noteDevices.removeAt(index);
		}

		void insertEffectDevice(size_t index, Devices::EffectDevicePtr effect) {
			_effectDevices.insertAt(index, effect);
		}

		void appendEffectDevice(Devices::EffectDevicePtr effect) {
			_effectDevices.append(effect);
		}

		void removeEffectDevice(size_t index) {
			_effectDevices.removeAt(index);
		}

		const ChannelNS::DeviceContainer<Devices::NoteDevicePtr> & getNoteDevices() {
			return _noteDevices;
		}

		const ChannelNS::DeviceContainer<Devices::EffectDevicePtr> & getEffectDevices() {
			return _effectDevices;
		}

		Devices::InstrumentDevicePtr getInstrument() {
			return _instrumentDevice;
		}

		void setInstrument(Devices::InstrumentDevicePtr instrument) {
			_instrumentDevice = instrument;
		}

		void clearInstrument() {
			_instrumentDevice = nullptr;
		}

		StereoSample update(const Transport &transport, std::list<NoteEvent> events);

		void addNoteEvent(NoteEvent event) {
			_events.push_back(event);
		}

		void insertSequence(uint64_t start, uint64_t length) {
			_track.insertSequence(start, length);
		}

		void eraseSequence(uint64_t position) {
			_track.eraseSequence(position);
		}

	private:
		std::string _name;
		Identifier _id;
		ChannelNS::DeviceContainer<Devices::NoteDevicePtr> _noteDevices;
		Devices::InstrumentDevicePtr _instrumentDevice;
		ChannelNS::DeviceContainer<Devices::EffectDevicePtr> _effectDevices;
		std::list<NoteEvent> _events;
		Track _track;
};

typedef std::shared_ptr<Channel> ChannelPtr;

#endif
