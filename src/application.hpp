#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "engine.hpp"
#include "argument_parser.hpp"
#include "websocket/server.hpp"
#include "message_handler.hpp"

class Application {
	public:
		Application(const ArgumentParser &);

		void start();
		void startTest(const ArgumentParser &);
		void stop();

		Engine & getEngine() {
			return _engine;
		}

	private:
		void run();
		void runTest(const std::string &, const std::vector<uint8_t> &, const std::map<std::string, int> &);

		bool _running = false;

		uint16_t _serverPort = 9000;
		Engine _engine;
		Websocket::Server _server;

		void processMessageQueue(Websocket::MessageQueue);

		MessageHandler _messageHandler;
};

#endif
