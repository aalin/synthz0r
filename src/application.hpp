#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "engine.hpp"
#include "argument_parser.hpp"
#include "websocket/message_queue.hpp"

class Application {
	public:
		Application(const ArgumentParser &, Websocket::MessageQueue &);

		void start();
		void stop();

		Engine & getEngine() {
			return _engine;
		}

	private:
		void run();

		bool _running = false;

		Engine _engine;
		Websocket::MessageQueue &_mq;
};

#endif
