#include <iostream>
#include <memory>
#include <list>
#include "engine.hpp"
#include "devices/synth.hpp"
#include "devices/wavetable_synth.hpp"
#include "devices/delay.hpp"
#include "devices/overdrive.hpp"
#include "devices/bitcrusher.hpp"
#include "utils.hpp"
#include "sequencer.hpp"
#include "note.hpp"
#include "performance_log.hpp"

#include "pulse_audio.hpp"
#include "file_output.hpp"

constexpr unsigned int BUFFER_SIZE = 1024 * 8;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;

int main(int, char *argv[]) {
	try {
		PerformanceLog perf;

		auto buffer = std::make_shared<AudioBuffer32Bit>(NUM_CHANNELS, BUFFER_SIZE);

		perf.log("Created audio buffer");

		//auto output = std::make_shared<FileOutput>("out.wav", buffer->sampleFormat(), SAMPLE_RATE, NUM_CHANNELS);
		auto output = std::make_shared<PulseAudio>(argv[0], buffer->sampleFormat(), SAMPLE_RATE, NUM_CHANNELS);

		perf.log("Created output");

		Engine engine(SAMPLE_RATE, buffer, output);

		perf.log("Created engine");

		engine.start();
		perf.log("Created output");

		auto snare = std::make_shared<Devices::Synth>();

		perf.log("Created snare");

		snare->set("oscillatorType", Oscillator::Type::NOISE);
		snare->set("amplitude", 10);
		snare->set("transpose", 0);
		snare->set("envelope.attackMs", 50);
		snare->set("envelope.decayMs", 100);
		snare->set("envelope.sustain", 0);
		snare->set("envelope.releaseMs", 50);
		snare->set("filter.enabled", 0);
		snare->set("filter.type", 1);
		snare->set("filter.cutoffHz", 3000);
		snare->set("filter.resonance", 300);

		perf.log("Set snare params");

		engine.addDevice(snare);

		perf.log("Adding snare to engine");

		auto snareDelay = std::make_shared<Devices::Delay>(150, 100, 50);

		perf.log("Created snare delay effect");

		snare->outputs()
			.add(snareDelay)->outputs()
			.add(engine.getOutputDevice());

		perf.log("Route snare outputs");

		auto wavetableSynth = std::make_shared<Devices::WavetableSynth>();
		wavetableSynth->set("amplitude", 10);
		wavetableSynth->set("transpose", 0);
		wavetableSynth->set("panning", 127);
		wavetableSynth->set("envelope.attackMs", 50);
		wavetableSynth->set("envelope.decayMs", 100);
		wavetableSynth->set("envelope.sustain", 100);
		wavetableSynth->set("envelope.releaseMs", 50);

		wavetableSynth->outputs()
			.add(engine.getOutputDevice());

		engine.addDevice(wavetableSynth);

		Sequencer snareSeq(4, 1.0, 1.0);

		snareSeq
			.setStep(0, NOTE(C,4))
			.setStep(2, NOTE(C,4))
			.setStep(3, NOTE(C,4));

		auto synth1 = std::make_shared<Devices::Synth>();

		synth1->setName("Synth 1");

		for (const auto &param : synth1->parameters()) {
			std::cout << param << std::endl;
		}

		synth1->set("oscillatorType", Oscillator::Type::SAW);
		synth1->set("panning", -127);
		synth1->set("amplitude", 50);
		synth1->set("transpose", 12);
		synth1->set("envelope.attackMs", 100);
		synth1->set("envelope.decayMs", 300);
		synth1->set("envelope.sustain", 0);
		synth1->set("envelope.releaseMs", 50);

		synth1->set("filter.enabled", 1);
		synth1->set("filter.type", 1);
		synth1->set("filter.cutoffHz", 5000);
		synth1->set("filter.resonance", 400);
		synth1->set("filter.bandwidth", 450);

		synth1->outputs()
		//	.add(std::make_shared<Devices::Overdrive>(32, 100))->outputs()
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

		auto synth2 = std::make_shared<Devices::Synth>();

		synth2->setName("Synth 2");

		synth2->set("oscillatorType", Oscillator::Type::SAW);
		synth2->set("amplitude", 100);
		synth2->set("transpose", -12 * 3);
		synth2->set("envelope.attackMs", 150);
		synth2->set("envelope.decayMs", 250);
		synth2->set("envelope.sustain", 100);
		synth2->set("envelope.releaseMs", 50);

		synth2->set("filter.cutoffHz", 5000);
		synth2->set("filter.resonance", 200);

		synth2->outputs()
			//.add(std::make_shared<Devices::Bitcrusher>(4, 20))->outputs()
			.add(engine.getOutputDevice());

		engine.addDevice(synth2);

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
			std::cout << "Time: " << timer.seconds() << std::endl;

			const int curr = timer.seconds() / 2;

			if (curr != prev) {
				wavetableSynth->set("waveformIndex", (int)timer.seconds() % 19);
				std::cout << "Using waveform " << wavetableSynth->getWaveformName() << std::endl;

				wavetableSynth->noteOff(timer, 60 + Utils::mod(prev, 12));
				wavetableSynth->noteOn(timer, 60 + Utils::mod(curr, 12));

				synth1->noteOff(timer, 60 - 12);
				synth1->noteOn(timer, 60 - 12);
				prev = curr;
			}

/*
			synth1->set("panning", Utils::rsin(timer.seconds(), -127, 127));

			snareSeq.setSpeed(8.0);
			sequencer1.setSpeed(2.0);
			sequencer2.setSpeed(4.0);

			sequencer1.update(timer, synth1);
			sequencer2.update(timer, synth2);
			snareSeq.update(timer, snare);

			*/
			engine.update();

			if (timer.seconds() > 60) {
				engine.stop();
			}
		}
	} catch (const char *msg) {
		std::cerr << "Error: " << msg << std::endl;
		return 1;
	}

	return 0;
}
