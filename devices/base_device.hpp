#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <memory>
#include "../timer.hpp"

namespace Devices {

class BaseDevice;
typedef std::shared_ptr<BaseDevice> DevicePtr;

class BaseDevice {
	public:
		BaseDevice()
		: _time(0.0)
		{ }

		virtual float tick(const Timer &timer) {
			_time = timer.getSeconds();
		}

		virtual float update(const Timer &timer, float pitchBend = 0.0) {
			return 0.0;
		}

		void addOutput(DevicePtr output) {
			_outputs.push_back(output);
		}

		void removeOutput(DevicePtr output) {
			// TODO: Implement me
		}

	private:
		float _time;
		std::list<DevicePtr> _outputs;

		void send(const Timer &timer, float value) {
			for (auto output : _outputs) {
				output->update(timer, value);
			}
		}
};

};

#endif
