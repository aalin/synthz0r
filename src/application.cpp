#include <iostream>
#include <memory>
#include "application.hpp"

#include "devices/factory.hpp"

#include "utils.hpp"
#include "note.hpp"

#include "file_output.hpp"

constexpr unsigned int BUFFER_SIZE = 512;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;
constexpr int DEFAULT_PORT = 5555;

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

std::list<AudioOutputPtr> setupOutputs(const ArgumentParser &args, AudioBufferPtr buffer) {
	std::list<AudioOutputPtr> outputs;

	outputs.push_back(
		std::make_shared<PulseAudio>(
			args.get(0).c_str(),
			buffer->sampleFormat(),
			SAMPLE_RATE,
			NUM_CHANNELS
		)
	);

	const auto filename = args.get("-f");

	if (filename.found) {
		outputs.push_back(
			std::make_shared<FileOutput>(
				filename.value,
				buffer->sampleFormat(),
				SAMPLE_RATE,
				NUM_CHANNELS
			)
		);
	}

	return outputs;
}

Engine setupEngine(const ArgumentParser &args) {
	auto buffer = std::make_shared<AudioBuffer32Bit>(NUM_CHANNELS, BUFFER_SIZE);
	auto outputs = setupOutputs(args, buffer);

	return Engine(SAMPLE_RATE, buffer, outputs);
}

Application::Application(const ArgumentParser &args)
: _engine(setupEngine(args)),
  _server(Websocket::Server())
{
	_serverPort = getPort(args);
}

void Application::start() {
	if (_running) {
		return;
	}

	_running = true;

	_server.start(_serverPort);
	run();
}

void Application::stop() {
	_running = false;
	_server.stop();
}

void Application::run() {
	Engine &engine = getEngine();

	try {
		int seconds = -1;

		while (_running) {
			int now = engine.transport().secondsElapsedSinceStart();

			if (now > seconds) {
				std::cout << "Timer: " << now << std::endl;
				seconds = now;
			}

			processMessageQueue(_server.update());

			engine.update();

			if (_engine.hasRequestedExit()) {
				stop();
			}
		}
	} catch (std::exception &e) {
		std::cerr << "Caught error: " << e.what() << std::endl;
	} catch (const char *msg) {
		std::cerr << "Caught error: " << msg << std::endl;
	}
}

void Application::processMessageQueue(Websocket::MessageQueue messages) {
	while (!messages.empty()) {
		_messageHandler.handleMessage(_engine, std::move(messages.front()));
		messages.pop();
	}
}
