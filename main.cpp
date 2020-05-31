#include <iostream>
#include <memory>
#include <vector>
#include "engine.hpp"
#include "pulse_audio.hpp"
#include "synth.hpp"
#include "engine.hpp"
#include "bitcrusher.hpp"
#include "utils.hpp"
#include "sequencer.hpp"

constexpr unsigned int BUFFER_SIZE = 1024;
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

		auto synth1 = std::make_shared<Synth>(Oscillator::Type::SAW);

		synth1->amplitude = 1.0;
		synth1->transpose = 0;
		synth1->envelope
			.setAttack(0.15)
			.setDecay(0.5)
			.setSustain(0.2)
			.setRelease(0.05);

		synth1->addEffect(std::make_shared<Bitcrusher>(8));

		engine.addDevice(synth1);

		auto synth2 = std::make_shared<Synth>(Oscillator::Type::SQUARE);

		synth2->amplitude = 0.4;
		synth2->transpose = 0;
		synth2->envelope
			.setAttack(0.15)
			.setDecay(0.55)
			.setSustain(0.2)
			.setRelease(0.05);
		engine.addDevice(synth2);

		Sequencer sequencer1(8, 1.0, 1.0);

		sequencer1
			.setStep(0, 52)
			.setStep(1, 55)
			.setStep(2, 57)
			.setStep(3, 55)
			.setStep(4, 62)
			.setStep(5, 60)
			.setStep(6, 62)
			.setStep(7, 64);

		Sequencer sequencer2(8, 1.0, 1.0);

		sequencer2
			.setStep(0, 52)
			.setStep(1, 55)
			.setStep(2, 57)
			.setStep(3, 55)
			.setStep(4, 62)
			.setStep(5, 60)
			.setStep(6, 62)
			.setStep(7, 64);

		while (engine.running()) {
			const float time = engine.getScaledTime();
			std::cout << "Time: " << time << std::endl;

			sequencer1.setSpeed(8.0 + std::sin(time / 4.0) / 2.0);
			sequencer1.update(synth1, time);
			sequencer2.setSpeed(7.0 + std::sin(time / 3.0) / 2.0);
			sequencer2.update(synth2, time);

			engine.update();
		}
	} catch (const char *msg) {
		std::cerr << "Error: " << msg << std::endl;
		return 1;
	}

	return 0;
}
