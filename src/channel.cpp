#include "channel.hpp"

StereoSample Channel::update(const Transport &transport, std::list<NoteEvent> events) {
	events.insert(events.end(), _events.begin(), _events.end());
	_events.clear();

	if (transport.position().ticksChanged()) {
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

	StereoSample out = _instrumentDevice->apply(transport, events);

	for (auto effectDevice : _effectDevices) {
		out = effectDevice->apply(transport, out, events);
	}

	return out;
}
