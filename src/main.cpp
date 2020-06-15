#include <iostream>
#include <thread>
#include "application.hpp"
#include "argument_parser.hpp"
#include "websocket/server.hpp"

constexpr int DEFAULT_PORT = 9000;

bool printHelp(const ArgumentParser &args) {
	if (args.has("-h")) {
		std::cout <<
			"Usage: " << args.get(0) << " [OPTION]..." << std::endl << std::endl <<
			"  -h                  Print help" << std::endl <<
			"  -f filename.wav     Export .wav to file" << std::endl <<
			"  -p 1234             Listen to port 1234" << std::endl;
		return true;
	}

	return false;
}

uint16_t getPort(const ArgumentParser &args) {
	auto option = args.get("-p");

	if (option.found) {
		const uint16_t port = atoi(option.value.c_str());

		if (port != 0) {
			return port;
		}
	}

	return DEFAULT_PORT;
}

int main(int argc, char *argv[]) {
	ArgumentParser args(argc, argv);

	if (printHelp(args)) {
		return 0;
	}

	const auto port = getPort(args);
	std::cout << "Port: " << port << std::endl;

	Websocket::MessageQueue mq;

	Application app(args, mq);
	Websocket::Server server(mq);

	std::thread appThread(&Application::start, &app);
	std::thread serverThread(&Websocket::Server::start, &server, getPort(args));

	appThread.join();
	serverThread.join();

	return 0;
}
