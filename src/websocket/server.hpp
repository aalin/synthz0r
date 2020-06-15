#ifndef WEBSOCKET__SERVER_HPP
#define WEBSOCKET__SERVER_HPP

#include <string>
#include "message_queue.hpp"

namespace Websocket {
	class ServerPimpl {
		public:
			virtual void start(uint16_t) = 0;
			virtual void stop() = 0;
			virtual void broadcast(const std::string &message) = 0;
	};

	class Server {
		public:
			Server(MessageQueue &mq);

			void start(uint16_t port) {
				_serverPimpl->start(port);
			}

			void stop() {
				_serverPimpl->stop();
			}

			void broadcast(const std::string &message) {
				_serverPimpl->broadcast(message);
			}

		private:
			std::unique_ptr<ServerPimpl> _serverPimpl;
	};
}

#endif
