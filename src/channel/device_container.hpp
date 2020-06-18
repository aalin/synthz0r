#ifndef CHANNEL__DEVICE_CONTAINER_HPP
#define CHANNEL__DEVICE_CONTAINER_HPP

namespace ChannelNS {
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
}

#endif
