#include <iostream>
#include <memory>
#include "application.hpp"

#include "devices/synth.hpp"
#include "devices/wavetable_synth.hpp"
#include "devices/delay.hpp"
#include "devices/overdrive.hpp"
#include "devices/bitcrusher.hpp"
#include "devices/kickdrum.hpp"
#include "utils.hpp"
#include "sequencer.hpp"
#include "note.hpp"
#include "performance_log.hpp"

#include "file_output.hpp"

constexpr unsigned int BUFFER_SIZE = 1024 * 1;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;

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

Application::Application(const ArgumentParser &args, Websocket::MessageQueue &mq)
: _engine(setupEngine(args)),
  _mq(mq)
{}

void Application::start() {
	if (_engine.running()) {
		return;
	}

	_engine.start();

	run();
}

void Application::stop() {
	_engine.stop();
}

void Application::run() {
	Engine &engine = getEngine();

	try {
		PerformanceLog perf;

		perf.log("Created output");

		auto kick = std::make_shared<Devices::Kickdrum>();
		kick->setParam("amplitude", 100);
		engine.addDevice(kick);

		kick->outputs()
			//.add(std::make_shared<Devices::Overdrive>(52, 100))->outputs()
			.add(engine.getOutputDevice());
		perf.log("Created kick");

		Sequencer kickSeq(4, 1.0, 1.0);

		kickSeq
			.setStep(0, NOTE(C,4))
			.setStep(1, NOTE_OFF)
			.setStep(1, NOTE_OFF)
			.setStep(1, NOTE_OFF);

		auto snare = std::make_shared<Devices::Synth>();

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

		Sequencer snareSeq(4, 1.0, 1.0);

		snareSeq
			.setStep(0, NOTE(C,4))
			.setStep(1, NOTE_OFF)
			.setStep(2, NOTE(C,4))
			.setStep(3, NOTE(C,4));

		perf.log("Create snare sequencer");

		auto wavetableSynth = std::make_shared<Devices::WavetableSynth>();
		wavetableSynth->setParam("amplitude", 20);
		wavetableSynth->setParam("transpose", -12 * 2);
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

		Sequencer sequencer1(16, 1.0, 1.0);

		sequencer1
			.setStep(0, NOTE(G,4))
			.setStep(1, NOTE(G,4))
			.setStep(2, NOTE(D,5))
			.setStep(3, NOTE(D,5))
			.setStep(4, NOTE(E,5))
			.setStep(5, NOTE(E,5))
			.setStep(6, NOTE(D,5))
			.setStep(7, NOTE_OFF)
			.setStep(8, NOTE(C,5))
			.setStep(9, NOTE(C,5))
			.setStep(10, NOTE(B,4))
			.setStep(11, NOTE(B,4))
			.setStep(12, NOTE(A,4))
			.setStep(13, NOTE(A,4))
			.setStep(14, NOTE(G,4));

		Sequencer sequencer11(16, 1.0, 1.0);

		sequencer11
			.setStep(0, NOTE(G,4))
			.setStep(1, NOTE(G,4))
			.setStep(2, NOTE(D,5))
			.setStep(3, NOTE(D,5))
			.setStep(4, NOTE(E,5))
			.setStep(5, NOTE(E,5))
			.setStep(6, NOTE(D,5))
			.setStep(7, NOTE_OFF)
			.setStep(8, NOTE(C,5))
			.setStep(9, NOTE(C,5))
			.setStep(10, NOTE(B,4))
			.setStep(11, NOTE(B,4))
			.setStep(12, NOTE(A,4))
			.setStep(13, NOTE(A,4))
			.setStep(14, NOTE(G,4));

		auto bass = std::make_shared<Devices::Synth>();

		bass->setName("Synth 2");

		bass->setParam("oscillatorType", Units::Oscillator::Type::SQUARE);
		bass->setParam("amplitude", 5);
		bass->setParam("transpose", -12 * 2);
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

		Sequencer sequencer2(32, 1.0, 1.0);

		sequencer2
			.setStep(0, NOTE(G,4))
			.setStep(1, NOTE(G,5))
			.setStep(2, NOTE(G,4))
			.setStep(3, NOTE(G,5))
			.setStep(4, NOTE(D,4))
			.setStep(5, NOTE(D,5))
			.setStep(6, NOTE(D,4))
			.setStep(7, NOTE(D,5))
			.setStep(8, NOTE(E,4))
			.setStep(9, NOTE(E,5))
			.setStep(10, NOTE(E,4))
			.setStep(11, NOTE(E,5))
			.setStep(12, NOTE(D,4))
			.setStep(13, NOTE(D,3))
			.setStep(14, NOTE(D,4))
			.setStep(15, NOTE(D,5))
			.setStep(16, NOTE(C,4))
			.setStep(17, NOTE(C,5))
			.setStep(18, NOTE(C,4))
			.setStep(19, NOTE(C,5))
			.setStep(20, NOTE(B,3))
			.setStep(21, NOTE(B,4))
			.setStep(22, NOTE(B,3))
			.setStep(23, NOTE(B,4))
			.setStep(24, NOTE(A,3))
			.setStep(25, NOTE(A,4))
			.setStep(26, NOTE(A,3))
			.setStep(27, NOTE(A,4))
			.setStep(28, NOTE(G,3))
			.setStep(29, NOTE(G,2))
			.setStep(30, NOTE(G,3))
			.setStep(31, NOTE(G,4));

		int prev = -1;

		while (engine.running()) {
			const Timer &timer = engine.timer();
			const int curr = timer.seconds();

			if (curr != prev) {
				std::cout << "Time: " << timer.seconds() << std::endl;
				wavetableSynth->setParam("waveformIndex", (curr / 2) % Waveform::WAVEFORMS.size());
				std::cout << "Using waveform " << wavetableSynth->getWaveformName() << std::endl;

//				wavetableSynth->noteOff(timer, 60 + Utils::mod(prev, 12));
//				wavetableSynth->noteOn(timer, 60 + Utils::mod(curr, 12));

//				synth1->noteOff(timer, 60 - 12);
//				synth1->noteOn(timer, 60 - 12);
				prev = curr;
			}

			synth1->setParam("panning", Utils::rsin(timer.seconds(), -127, 127));

			kickSeq.setSpeed(8.0);
			snareSeq.setSpeed(4.0);
			sequencer1.setSpeed(2.0);
			sequencer11.setSpeed(2.0);
			sequencer2.setSpeed(4.0);

			sequencer1.tick(timer, synth1);
			sequencer11.tick(timer, wavetableSynth);
			sequencer2.tick(timer, bass);
			snareSeq.tick(timer, snare);
			kickSeq.tick(timer, kick);

			engine.update();

			if (timer.seconds() > 60) {
				engine.stop();
			}
		}
	} catch (const char *msg) {
		std::cerr << "Error: " << msg << std::endl;
	}
}
