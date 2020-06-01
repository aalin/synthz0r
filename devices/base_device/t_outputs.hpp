#ifndef OUTPUTS_HPP
#define OUTPUTS_HPP

namespace Devices {

class BaseDevice;

template<typename T>
class TOutputs {
	public:
		const std::list<T> & list() const {
			return _outputs;
		}

		TOutputs<T> & add(T output) {
			_outputs.push_back(output);
			return *this;
		}

		TOutputs<T> & remove(T output) {
			for (auto it = _outputs.begin(); it != _outputs.end();) {
				if (output == *it) {
					it = _outputs.erase(it);
				} else {
					++it;
				}
			}

			return *this;
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

		friend class BaseDevice;
};
};

#endif
