#ifndef DEVICES__BASE_DEVICE__T_OUTPUTS_HPP
#define DEVICES__BASE_DEVICE__T_OUTPUTS_HPP

#include <list>
#include "../../stereo_sample.hpp"
#include "../../timer.hpp"
#include "../../note_event.hpp"

namespace Devices {
class BaseDevice;

template<typename T>
class TOutputs {
	public:
		const std::list<T> & list() const {
			return _outputs;
		}

		T add(T output) {
			_outputs.push_back(output);
			return output;
		}

		void remove(T output) {
			for (auto it = _outputs.begin(); it != _outputs.end();) {
				if (output == *it) {
					it = _outputs.erase(it);
				} else {
					++it;
				}
			}
		}

		const std::list<T>::const_iterator begin() const {
			return _outputs.begin();
		}

		const std::list<T>::const_iterator end() const {
			return _outputs.end();
		}

	private:
		std::list<T> _outputs;

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

		void output(const Timer &timer, const NoteEvent &value) {
			for (auto output : _outputs) {
				output->input(timer, value);
			}
		}

		friend class BaseDevice;
};
}

#endif
