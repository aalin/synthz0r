#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include "engine.hpp"
#include "websocket/message.hpp"

class MessageHandler {
	public:
		void handleMessage(Engine &engine, Websocket::MessagePtr message);
};

#endif
