#include <iostream>
#include <csignal>
#include "application.hpp"
#include "argument_parser.hpp"

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

std::function<void(int)> shutdownHandler;

void signalHandler(int signal) {
	if (!shutdownHandler) {
		std::cerr << "Got SIGINT but got no shutdown handler" << std::endl;
		return;
	}

	shutdownHandler(signal);
}

struct {
	typedef std::chrono::time_point<std::chrono::steady_clock> time_point;

	time_point lastTime = std::chrono::steady_clock::now();

	bool shouldTerminate() {
		time_point now = std::chrono::steady_clock::now();

		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();

		lastTime = now;

		return diff < 500;
	}

} InterruptTimer;

int main(int argc, char *argv[]) {
	ArgumentParser args(argc, argv);

	if (printHelp(args)) {
		return 0;
	}

	Application app(args);

	bool isTerminating = false;

	shutdownHandler = [&](int) -> void {
		if (isTerminating) {
			std::cout << "Terminating..." << std::endl;
			return;
		}

		if (InterruptTimer.shouldTerminate()) {
			isTerminating = true;
			app.stop();
		} else {
			std::cout << "Press Ctrl+C again if you want to terminate" << std::endl;
		}
	};

	std::signal(SIGINT, signalHandler);

	app.start();

	return 0;
}
