#include <iostream>
#include <memory>
#include <list>
#include "engine.hpp"
#include "devices/synth.hpp"
#include "devices/delay.hpp"
#include "devices/overdrive.hpp"
#include "devices/bitcrusher.hpp"
#include "utils.hpp"
#include "sequencer.hpp"
#include "note.hpp"

constexpr unsigned int BUFFER_SIZE = 1024 * 1;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;

int main(int, char *argv[]) {
	try {
		auto buffer = std::make_shared<AudioBuffer32Bit>(NUM_CHANNELS, BUFFER_SIZE);
		Engine engine(argv[0], SAMPLE_RATE, buffer);

		engine.start();

		auto synth1 = std::make_shared<Devices::Synth>(Oscillator::Type::SINE);

		synth1->setName("Synth 1");

		for (const auto &param : synth1->parameters()) {
			std::cout << param << std::endl;
		}

		synth1->set("amplitude", 100);
		synth1->set("transpose", 0);
		synth1->set("envelope.attackMs", 100);
		synth1->set("envelope.decayMs", 100);
		synth1->set("envelope.sustain", 0);
		synth1->set("envelope.releaseMs", 50);

		synth1->set("filter.cutoffHz", 4000);
		synth1->set("filter.resonance", 1000);
		synth1->set("filter.bandwidth", 500);
		synth1->set("filter.type", 1);

		synth1->outputs()
			.add(std::make_shared<Devices::Overdrive>(64, 128))->outputs()
			.add(std::make_shared<Devices::Delay>(250, 100, 90))->outputs()
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

		auto synth2 = std::make_shared<Devices::Synth>(Oscillator::Type::SAW);

		synth1->setName("Synth 2");

		synth2->set("amplitude", 100);
		synth2->set("transpose", -12 * 3);
		synth2->set("envelope.attackMs", 150);
		synth2->set("envelope.decayMs", 250);
		synth2->set("envelope.sustain", 100);
		synth2->set("envelope.releaseMs", 50);

		synth1->set("filter.cutoffHz", 7000);
		synth1->set("filter.resonance", 500);
		synth1->set("filter.bandwidth", 500);
		synth1->set("filter.type", 0);

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

		while (engine.running()) {
			const Timer &timer = engine.timer();
			std::cout << "Time: " << timer.seconds() << std::endl;

			synth1->set("panning", Utils::rsin(timer.seconds(), -127, 127));

			synth2->set("filter.cutoffHz", Utils::rsin(timer.seconds() * 2.0, 2000, 10000));
			synth2->set("filter.resonance", Utils::rsin(timer.seconds() / 2.0, 0, 1000));
			synth2->set("filter.bandwidth", Utils::rsin(timer.seconds() / 4.0, 0, 1000));

			sequencer1.setSpeed(2.0);
			sequencer2.setSpeed(4.0);

			sequencer1.update(timer, synth1);

			sequencer2.update(timer, synth2);

			engine.update();
		}
	} catch (const char *msg) {
		std::cerr << "Error: " << msg << std::endl;
		return 1;
	}

	return 0;
}
