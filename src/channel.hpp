#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include "devices.hpp"
#include "timer.hpp"
#include "channel/device_container.hpp"

class Channel {
	public:
		Channel(std::string name) : _name(name) {
			std::cout << "Creating channel: " << _name << " id: " << _id.get() << std::endl;
		}

		~Channel() {
			std::cout << "Destroying channel: " << _name << " id: " << _id.get() << std::endl;
		}

		uint32_t id() const { return _id; }

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

		void setInstrument(Devices::InstrumentDevicePtr instrument) {
			_instrumentDevice = instrument;
		}

		void clearInstrument() {
			_instrumentDevice = nullptr;
		}

		StereoSample update(const Timer &timer, std::list<NoteEvent> events) {
			if (_instrumentDevice == nullptr) {
				return StereoSample();
			}

			for (auto noteDevice : _noteDevices) {
				noteDevice->apply(timer, events);
			}

			StereoSample out = _instrumentDevice->apply(timer, events);

			for (auto effectDevice : _effectDevices) {
				out = effectDevice->apply(timer, out, events);
			}

			return out;
		}

	private:
		Identifier _id;
		std::string _name;
		ChannelNS::DeviceContainer<Devices::NoteDevicePtr> _noteDevices;
		Devices::InstrumentDevicePtr _instrumentDevice;
		ChannelNS::DeviceContainer<Devices::EffectDevicePtr> _effectDevices;
};

typedef std::shared_ptr<Channel> ChannelPtr;

#endif
