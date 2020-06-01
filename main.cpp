#include <iostream>
#include <memory>
#include <vector>
#include "engine.hpp"
#include "pulse_audio.hpp"
#include "synth.hpp"
#include "engine.hpp"
#include "effects/bitcrusher.hpp"
#include "effects/overdrive.hpp"
#include "effects/delay.hpp"
#include "utils.hpp"
#include "sequencer.hpp"
#include "note.hpp"

constexpr unsigned int BUFFER_SIZE = 1024 * 1;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;

typedef AudioBuffer<uint8_t> AudioBuffer8Bit;
typedef AudioBuffer<int16_t> AudioBuffer16Bit;
typedef AudioBuffer<int32_t> AudioBuffer32Bit;
typedef AudioBuffer<float> AudioBufferFloat;

int main(int argc, char *argv[]) {
	try {
		auto buffer = std::make_shared<AudioBuffer32Bit>(NUM_CHANNELS, BUFFER_SIZE);
		Engine engine(argv[0], SAMPLE_RATE, buffer);

		engine.start();

		auto synth1 = std::make_shared<Synth>(Oscillator::Type::SINE);

		synth1->amplitude = 1.0;
		synth1->transpose = 0;
		synth1->envelope
			.setAttack(0.2)
			.setDecay(0.1)
			.setSustain(0.0)
			.setRelease(0.05);

		synth1->addEffect(std::make_shared<Effects::Overdrive>(8));
		synth1->addEffect(std::make_shared<Effects::Delay>(250, 0.8, 0.8, SAMPLE_RATE));
		synth1->addEffect(std::make_shared<Effects::Delay>(125, 0.5, 0.5, SAMPLE_RATE));
		// synth1->addEffect(std::make_shared<Effects::Bitcrusher>(6));

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

		auto synth2 = std::make_shared<Synth>(Oscillator::Type::SQUARE);

		synth2->amplitude = 0.1;
		synth2->transpose = -12;
		synth2->envelope
			.setAttack(0.15)
			.setDecay(0.25)
			.setSustain(0.1)
			.setRelease(0.05);

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
			.setStep(13, NOTE(D,5))
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
			.setStep(29, NOTE(G,4))
			.setStep(30, NOTE(G,3))
			.setStep(31, NOTE(G,4));

		while (engine.running()) {
			const float time = engine.getScaledTime();
			std::cout << "Time: " << time << std::endl;

			sequencer1.setSpeed(2.0);
			sequencer1.update(synth1, time);
			sequencer2.setSpeed(4.0);
			sequencer2.update(synth2, time);

			engine.update();
		}
	} catch (const char *msg) {
		std::cerr << "Error: " << msg << std::endl;
		return 1;
	}

	return 0;
}
