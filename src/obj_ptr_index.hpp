#ifndef OBJ_PTR_INDEX_HPP
#define OBJ_PTR_INDEX_HPP

template<typename T>
class ObjPtrIndex {
	public:
		void add(std::shared_ptr<T> objectPtr) {
			_map[objectPtr->id()] = objectPtr;
			cleanExpired();
		}

		std::shared_ptr<T> get(uint32_t id) {
			cleanExpired();

			auto it = _map.find(id);

			if (it == _map.end()) {
				return nullptr;
			}

			return it->second.lock();
		}

	private:
		std::map<uint32_t, std::weak_ptr<T> > _map;

		void cleanExpired() {
			auto it = _map.begin();

			while (it != _map.end()) {
				if (it->second.expired()) {
					it = _map.erase(it);
				} else {
					it++;
				}
			}
		}
};

#endif
