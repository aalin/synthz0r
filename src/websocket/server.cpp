#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>
#include "server.hpp"
#include "server/server_message.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
typedef server::message_ptr message_ptr;

class WSServer : public Websocket::ServerPimpl {
	public:
		WSServer() {
			_server.init_asio();

			_server.clear_access_channels(websocketpp::log::alevel::all);

			_server.set_open_handler(bind(&WSServer::onOpen, this, ::_1));
			_server.set_close_handler(bind(&WSServer::onClose, this, ::_1));
			_server.set_message_handler(bind(&WSServer::onMessage, this, ::_1, ::_2));
		}

		~WSServer() {
			std::cout << "Destroying server impl" << std::endl;;
		}

		void start(uint16_t port) {
			std::cout << "Server listening to port " << port << std::endl;
			_server.set_reuse_addr(true);
			_server.listen(port);
			_server.start_accept();
		}

		void stop() {
			if (!_server.is_listening()) {
				return;
			}

			std::cout << "Stopping server" << std::endl;

			_server.stop_listening();

			for (auto connection : _connections) {
				std::cout << "Closing connection" << std::endl;
				_server.close(connection, websocketpp::close::status::normal, "Terminating");
			}
		}

		void broadcast(const std::string &message) {
			for (auto connection : _connections) {
				_server.send(
					connection,
					message,
					websocketpp::frame::opcode::binary
				);
			}
		}

		void onOpen(connection_hdl hdl) {
			_connections.insert(hdl);
		}

		void onClose(connection_hdl hdl) {
			_connections.erase(hdl);
		}

		void onMessage(connection_hdl hdl, message_ptr msg) {
			_mq.push(
				std::make_unique<Websocket::ServerMessage>(
					msg->get_payload(),
					&_server,
					hdl
				)
			);
		}

		Websocket::MessageQueue update() {
			_server.poll();

			Websocket::MessageQueue messages;
			_mq.swap(messages);
			return messages;
		}

	private:
		typedef std::set<connection_hdl, std::owner_less<connection_hdl>> ConnectionList;

		server _server;
		ConnectionList _connections;
		Websocket::MessageQueue _mq;
};

Websocket::Server::Server()
: _serverPimpl(std::make_unique<WSServer>())
{}

Websocket::Server::~Server() {
	std::cout << "Destroying server" << std::endl;
}
