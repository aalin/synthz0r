#include <iostream>
#include <memory>

#include <sstream>
#include <iterator>
#include <iomanip>

#include "application.hpp"

#include "devices/factory.hpp"

#include "utils.hpp"
#include "note.hpp"

#include "audio/file_output.hpp"

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

std::list<Audio::AudioOutputPtr> setupOutputs(const ArgumentParser &args, Audio::AudioBufferPtr buffer) {
	std::list<Audio::AudioOutputPtr> outputs;

	outputs.push_back(
		std::make_shared<Audio::PulseAudio>(
			args.get(0).c_str(),
			buffer->sampleFormat(),
			SAMPLE_RATE,
			NUM_CHANNELS
		)
	);

	const auto filename = args.get("-f");

	if (filename.found) {
		outputs.push_back(
			std::make_shared<Audio::FileOutput>(
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
	auto buffer = std::make_shared<Audio::AudioBuffer32Bit>(NUM_CHANNELS, BUFFER_SIZE);
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

void Application::startTest(const ArgumentParser &args) {
	if (_running) {
		return;
	}

	_running = true;

	std::string instrumentName = args.get("-test").value;
	std::vector<uint8_t> notes;

	const auto notesOption = args.get("-notes");

	if (notesOption.found) {
		std::string str = notesOption.value;
		std::replace(str.begin(), str.end(), ',', ' ');
		std::istringstream iss(str);

		std::copy(
			std::istream_iterator<uint16_t>(iss),
			std::istream_iterator<uint16_t>(),
			std::back_inserter(notes)
		);
	}

	if (notes.empty()) {
		std::cout << "Using default notes" << std::endl;
		notes.assign({ 60, 64, 67, 60, 64, 67 });
	}

	runTest(instrumentName, notes);
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

void Application::runTest(const std::string &instrumentName, const std::vector<uint8_t> &notes) {
	Engine &engine = getEngine();

	auto channel = engine.createChannel("Test channel");

	auto instrument = Devices::Factory::createInstrumentDevice(instrumentName);

	if (!instrument) {
		std::cerr << "Could not find instrument" << std::endl;
		stop();
		return;
	}

	std::cout << std::endl <<
		"\x1b[1;4m" <<
		std::setw(21) << std::left <<
		"Parameter" << std::right <<
		std::setw(8) << "Value " <<
		std::setw(8) << "Min " <<
		std::setw(8) << "Max " <<
		std::setw(9) << "Default" <<
		"\x1b[0m " <<
		std::endl;

	for (auto &param : instrument->parameters()) {
		std::cout <<
			std::setw(20) << std::left <<
			param.name() << " " << std::right <<
			"\x1b[1;33m" <<
			std::setw(7) << param.value() << " " <<
			"\x1b[0m" <<
			std::setw(7) << param.min() << " " <<
			std::setw(7) << param.max() << " " <<
			std::setw(9) << param.defaultValue() <<
			std::endl;
	}

	std::cout << std::endl;

	channel->setInstrument(instrument);

	unsigned int index = 0;

	while (_running) {
		unsigned int now = engine.transport().secondsElapsedSinceStart() + 1;

		if (now - 1 > notes.size()) {
			stop();
			break;
		}

		if (now > index) {
			std::cout << "Timer: " << now << std::endl;

			if (index > 0){
				channel->addNoteEvent(NoteEvent::noteOff(notes[index - 1]));
			}

			if (index < notes.size()) {
				channel->addNoteEvent(NoteEvent::noteOn(notes[index]));
			}

			index = now;
		}

		engine.update();
	}
}

void Application::processMessageQueue(Websocket::MessageQueue messages) {
	while (!messages.empty()) {
		_messageHandler.handleMessage(_engine, std::move(messages.front()));
		messages.pop();
	}
}
