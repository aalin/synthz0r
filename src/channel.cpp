#include "channel.hpp"

StereoSample Channel::update(const Transport &transport, std::list<NoteEvent> events) {
	events.insert(events.end(), _events.begin(), _events.end());
	_events.clear();

	if (transport.shouldTriggerNotes()) {
		_track.getEventsAt(events, transport.position().totalTicks());
	}

	for (auto &event : events) {
		std::cout << "Channel " << id() << " event " << event << std::endl;
	}

	if (_instrumentDevice == nullptr) {
		return StereoSample();
	}

	for (auto noteDevice : _noteDevices) {
		noteDevice->apply(transport, events);
	}

	_instrumentDevice->handleEvents(transport, events);

	StereoSample out = _instrumentDevice->apply(transport);

	for (auto effectDevice : _effectDevices) {
		effectDevice->handleEvents(transport, events);
		out = effectDevice->apply(transport, out);
	}

	return out;
}
