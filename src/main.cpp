#include <iostream>
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

int main(int argc, char *argv[]) {
	ArgumentParser args(argc, argv);

	if (printHelp(args)) {
		return 0;
	}

	Application app(args);

	app.start();

	return 0;
}
