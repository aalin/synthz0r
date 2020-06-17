#ifndef WEBSOCKET__SERVER__WEBSOCKET_MESSAGE_HPP
#define WEBSOCKET__SERVER__WEBSOCKET_MESSAGE_HPP

#include "../message.hpp"

namespace Websocket {
	struct ServerMessage : public Message {
		typedef websocketpp::server<websocketpp::config::asio> server;

		ServerMessage(std::string payload, server *s, websocketpp::connection_hdl hdl)
		: Message(payload), s(s), hdl(hdl)
		{}

		void reply(std::string text) {
			try {
				s->send(hdl, text, websocketpp::frame::opcode::binary);
			} catch (websocketpp::exception const & e) {
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}

		private:
			server *s;
			websocketpp::connection_hdl hdl;
	};
}

#endif
