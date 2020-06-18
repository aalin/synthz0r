#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include "devices.hpp"
#include "timer.hpp"

class Channel {
	template<typename T>
	class DeviceContainer {
		public:
			typedef std::vector<T> ContainerType;

			size_t size() const;

			ContainerType::iterator begin() {
				return _elements.begin();
			}

			ContainerType::iterator end() {
				return _elements.end();
			}

			void prepend(T elem) {
				_elements.insert(_elements.begin(), elem);
			}

			void append(T elem) {
				_elements.push_back(elem);
			}

			void insertAt(size_t pos, T elem) {
				if (pos > _elements.size()) {
					_elements.insert(_elements.end(), elem);
					return;
				}

				_elements.insert(_elements.begin() + pos, elem);
			}

			void removeAt(size_t pos) {
				if (pos > _elements.size()) {
					return;
				}

				_elements.erase(_elements.begin() + pos);
			}

			void pop() {
				_elements.pop_back();
			}

		private:
			std::vector<T> _elements;
	};

	public:
		Channel(std::string name)
		: _name(name)
		{
			std::cout << "Creating channel: " << _name << std::endl;
		}

		~Channel() {
			std::cout << "Destroying channel: " << _name << std::endl;
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
		std::string _name;
		DeviceContainer<Devices::NoteDevicePtr> _noteDevices;
		Devices::InstrumentDevicePtr _instrumentDevice;
		DeviceContainer<Devices::EffectDevicePtr> _effectDevices;
};

typedef std::shared_ptr<Channel> ChannelPtr;

#endif
