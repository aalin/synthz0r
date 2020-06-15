#ifndef WEBSOCKET__MESSAGE_HPP
#define WEBSOCKET__MESSAGE_HPP

#include <string>
#include <memory>

namespace Websocket {
	struct Message {
		Message(std::string payload) : payload(payload) {}

		std::string payload;

		virtual void reply(std::string text) = 0;
	};

	typedef std::unique_ptr<Message> MessagePtr;
}

#endif
