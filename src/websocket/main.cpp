#include <iostream>
#include <thread>

#include "message_queue.hpp"
#include "server.hpp"

void appLoop(Websocket::MessageQueue &mq) {
	while (true) {
		mq.process([] (Websocket::MessagePtr msg) {
			std::cout << "appLoop received " << msg->payload << std::endl;
			msg->reply(msg->payload);
		});

		// std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main2() {
	Websocket::MessageQueue mq;
	std::thread appThread(appLoop, std::ref(mq));
	Websocket::Server server(mq);
	std::thread serverThread(&Websocket::Server::start, &server, 9001);
	serverThread.join();
	appThread.join();

	return 0;
}
