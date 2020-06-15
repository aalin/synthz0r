#include <iostream>
#include "application.hpp"
#include "argument_parser.hpp"

bool printHelp(const ArgumentParser &args) {
	if (args.has("-h")) {
		std::cout
			<< args.get(0) << " -h                  Print help" << std::endl
			<< args.get(0) << " -f filename.wav     Export to file" << std::endl
			<< args.get(0) << "                     Play with PulseAudio" << std::endl;
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
