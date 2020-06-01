#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <memory>
#include "../timer.hpp"
#include "../stereo_sample.hpp"

namespace Devices {

class BaseDevice;
typedef std::shared_ptr<BaseDevice> DevicePtr;

class BaseDevice {
	public:
		BaseDevice() { }

		virtual void update(const Timer &, float) { }
		virtual void update(const Timer &) { }
		virtual const char * name() const { return "BaseDevice"; }

		void addOutput(DevicePtr output) {
			_outputs.push_back(output);
		}

		void removeOutput(DevicePtr output) {
			for (auto it = _outputs.begin(); it != _outputs.end();) {
				if (output == *it) {
					it = _outputs.erase(it);
				} else {
					++it;
				}
			}
		}

		virtual void input(const Timer &, const float &) {
			throw "BaseDevice received input";
		}

		virtual void input(const Timer &, const StereoSample &) {
			throw "BaseDevice received input";
		}

	protected:
		void output(const Timer &timer, const float &value) {
			for (auto output : _outputs) {
				output->input(timer, value);
			}
		}

		void output(const Timer &timer, const StereoSample &value) {
			for (auto output : _outputs) {
				output->input(timer, value);
			}
		}

	private:
		std::list<DevicePtr> _outputs;
};
};

#endif
