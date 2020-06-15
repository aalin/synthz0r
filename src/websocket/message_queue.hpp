#ifndef WEBSOCKET__MESSAGE_QUEUE_HPP
#define WEBSOCKET__MESSAGE_QUEUE_HPP

#include <mutex>
#include <queue>
#include <functional>
#include "message.hpp"

namespace Websocket {
	class MessageQueue {
		public:
			typedef std::queue<MessagePtr> Queue;

			void push(MessagePtr message) {
				std::lock_guard<std::mutex> guard(_mutex);
				_messages.push(std::move(message));
			}

			void process(std::function<void(MessagePtr)> cb) {
				Queue messages(getAndClearMessages());

				while (!messages.empty()) {
					cb(std::move(messages.front()));
					messages.pop();
				}
			}

		private:
			Queue getAndClearMessages() {
				std::lock_guard<std::mutex> guard(_mutex);

				Queue messages;

				_messages.swap(messages);

				return messages;
			}

			std::mutex _mutex;
			Queue _messages;
	};
}

#endif
