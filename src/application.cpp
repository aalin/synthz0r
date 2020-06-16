#include <iostream>
#include <memory>
#include "application.hpp"

#include "devices/synth.hpp"
#include "devices/wavetable_synth.hpp"
#include "devices/delay.hpp"
#include "devices/overdrive.hpp"
#include "devices/bitcrusher.hpp"
#include "devices/kickdrum.hpp"
#include "devices/sequencer.hpp"
#include "utils.hpp"
#include "note.hpp"
#include "performance_log.hpp"

#include "file_output.hpp"

constexpr unsigned int BUFFER_SIZE = 1024 * 1;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;
constexpr int DEFAULT_PORT = 9000;

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

std::shared_ptr<AudioOutput> getOutput(const ArgumentParser &args, AudioBufferPtr buffer) {
	const auto filename = args.get("-f");

	if (filename.found) {
		return std::make_shared<FileOutput>(filename.value, buffer->sampleFormat(), SAMPLE_RATE, NUM_CHANNELS);
	}

	return std::make_shared<PulseAudio>(args.get(0).c_str(), buffer->sampleFormat(), SAMPLE_RATE, NUM_CHANNELS);
}

Engine setupEngine(const ArgumentParser &args) {
	auto buffer = std::make_shared<AudioBuffer32Bit>(NUM_CHANNELS, BUFFER_SIZE);
	auto output = getOutput(args, buffer);

	return Engine(SAMPLE_RATE, buffer, output);
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
		PerformanceLog perf;

		perf.log("Created output");

		auto kick = std::make_shared<Devices::Kickdrum>();
		kick->setName("Kickdrum");
		kick->setParam("amplitude", 100);
		engine.addDevice(kick);

		kick->outputs()
			//.add(std::make_shared<Devices::Overdrive>(52, 100))->outputs()
			.add(engine.getOutputDevice());
		perf.log("Created kick");

		auto kickSeq = std::make_shared<Devices::Sequencer>();
		engine.addDevice(kickSeq);

		kickSeq->outputs().add(kick);
		kickSeq->setSteps({
			NOTE(C,4),
			NOTE_OFF,
			NOTE_OFF,
			NOTE_OFF,
		});

		auto snare = std::make_shared<Devices::Synth>();
		kick->setName("Snare");

		perf.log("Created snare");

		snare->setParam("oscillatorType", Units::Oscillator::Type::NOISE);
		snare->setParam("amplitude", 30);
		snare->setParam("transpose", 0);
		snare->setParam("envelope.attackMs", 50);
		snare->setParam("envelope.decayMs", 100);
		snare->setParam("envelope.sustain", 0);
		snare->setParam("envelope.releaseMs", 50);
		snare->setParam("filter.enabled", 0);
		snare->setParam("filter.type", 1);
		snare->setParam("filter.cutoffHz", 3000);
		snare->setParam("filter.resonance", 300);

		perf.log("Set snare params");

		engine.addDevice(snare);

		perf.log("Adding snare to engine");

		perf.log("Created snare delay effect");

		snare->outputs()
			.add(std::make_shared<Devices::Delay>(150, 100, 50))->outputs()
			.add(engine.getOutputDevice());

		perf.log("Route snare outputs");


		auto snareSeq = std::make_shared<Devices::Sequencer>();
		engine.addDevice(snareSeq);

		snareSeq->outputs().add(snare);
		snareSeq->setSteps({
			NOTE(C,4),
			NOTE_OFF,
			NOTE(C,4),
			NOTE(C,4)
		});

		perf.log("Create snare sequencer");

		auto wavetableSynth = std::make_shared<Devices::WavetableSynth>();
		wavetableSynth->setName("Wavetable synth");
		wavetableSynth->setParam("amplitude", 100);
		wavetableSynth->setParam("transpose", -12 * 1);
		wavetableSynth->setParam("panning", 127);
		wavetableSynth->setParam("envelope.attackMs", 500);
		wavetableSynth->setParam("envelope.decayMs", 100);
		wavetableSynth->setParam("envelope.sustain", 100);
		wavetableSynth->setParam("envelope.releaseMs", 50);

		wavetableSynth->outputs()
			.add(engine.getOutputDevice());

		engine.addDevice(wavetableSynth);

		auto synth1 = std::make_shared<Devices::Synth>();

		synth1->setName("Synth 1");

		for (const auto &param : synth1->parameters()) {
			std::cout << param << std::endl;
		}

		synth1->setParam("oscillatorType", Units::Oscillator::Type::SAW);
		synth1->setParam("panning", -127);
		synth1->setParam("amplitude", 20);
		synth1->setParam("transpose", 12);
		synth1->setParam("envelope.attackMs", 100);
		synth1->setParam("envelope.decayMs", 300);
		synth1->setParam("envelope.sustain", 0);
		synth1->setParam("envelope.releaseMs", 50);

		synth1->setParam("filter.enabled", 1);
		synth1->setParam("filter.type", 1);
		synth1->setParam("filter.cutoffHz", 5000);
		synth1->setParam("filter.resonance", 400);
		synth1->setParam("filter.bandwidth", 450);

		synth1->outputs()
			.add(std::make_shared<Devices::Overdrive>(32, 100))->outputs()
			.add(std::make_shared<Devices::Delay>(250, 100, 50))->outputs()
			.add(engine.getOutputDevice());

		engine.addDevice(synth1);

		auto sequencer1 = std::make_shared<Devices::Sequencer>(16, 200, 100);
		engine.addDevice(sequencer1);

		sequencer1->outputs().add(synth1);
		sequencer1->outputs().add(wavetableSynth);

		sequencer1->setSteps({
			NOTE(G,4), NOTE(G,4), NOTE(D,5), NOTE(D,5),
			NOTE(E,5), NOTE(E,5), NOTE(D,5), NOTE_OFF,
			NOTE(C,5), NOTE(C,5), NOTE(B,4), NOTE(B,4),
			NOTE(A,4), NOTE(A,4), NOTE(G,4), NOTE_OFF
		});

		auto bass = std::make_shared<Devices::Synth>();

		bass->setName("Synth 2");

		bass->setParam("oscillatorType", Units::Oscillator::Type::SQUARE);
		bass->setParam("amplitude", 5);
		bass->setParam("transpose", -12 * 1);
		bass->setParam("envelope.attackMs", 150);
		bass->setParam("envelope.decayMs", 250);
		bass->setParam("envelope.sustain", 100);
		bass->setParam("envelope.releaseMs", 50);

		bass->setParam("filter.enabled", 0);
		bass->setParam("filter.cutoffHz", 4000);
		bass->setParam("filter.resonance", 200);

		bass->outputs()
			//.add(std::make_shared<Devices::Bitcrusher>(4, 20))->outputs()
			.add(engine.getOutputDevice());

		engine.addDevice(bass);

		auto sequencer2 = std::make_shared<Devices::Sequencer>(32, 200, 100);
		engine.addDevice(sequencer2);

		sequencer2->outputs().add(bass);
		sequencer2->setSteps({
			NOTE(G,4), NOTE(G,5), NOTE(G,4), NOTE(G,5),
			NOTE(D,4), NOTE(D,5), NOTE(D,4), NOTE(D,5),
			NOTE(E,4), NOTE(E,5), NOTE(E,4), NOTE(E,5),
			NOTE(D,4), NOTE(D,3), NOTE(D,4), NOTE(D,5),
			NOTE(C,4), NOTE(C,5), NOTE(C,4), NOTE(C,5),
			NOTE(B,3), NOTE(B,4), NOTE(B,3), NOTE(B,4),
			NOTE(A,3), NOTE(A,4), NOTE(A,3), NOTE(A,4),
			NOTE(G,3), NOTE(G,2), NOTE(G,3), NOTE(G,4)
		});

		while (_running) {
			const Timer &timer = engine.timer();

			processMessageQueue(_server.update());

			synth1->setParam("panning", -127);
			wavetableSynth->setParam("panning", 127);

			const int bpm = Utils::rsin(timer.seconds() / 2.0, 90, 120);

			sequencer1->setParam("bpm", bpm);
			sequencer2->setParam("bpm", bpm);
			sequencer2->setParam("rate", 2);
			snareSeq->setParam("bpm", bpm);
			snareSeq->setParam("rate", 4);
			kickSeq->setParam("bpm", bpm);
			kickSeq->setParam("rate", 4);

			engine.update();

			if (timer.seconds() > 60) {
				stop();
			}
		}
	} catch (const char *msg) {
		std::cerr << "Error: " << msg << std::endl;
	}
}

void Application::processMessageQueue(Websocket::MessageQueue messages) {
	while (!messages.empty()) {
		processMessage(std::move(messages.front()));
		messages.pop();
	}
}

void Application::processMessage(Websocket::MessagePtr message) {
	std::cout << "Got message: " << message->payload << std::endl;
	message->reply(message->payload);
}
