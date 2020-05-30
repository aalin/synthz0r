#include <iostream>
#include <cmath>
#include <limits>
#include "pulse_audio.hpp"
#include "synth.hpp"

constexpr unsigned int BUFSIZE = 1024;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;

template<typename T, typename X> T clip(X value) {
	constexpr T min = std::numeric_limits<T>::min();
	constexpr T max = std::numeric_limits<T>::max();

	if (value < min) {
		return min;
	}

	if (value > max) {
		return max;
	}

	return value;
}

int main(int argc, char *argv[]) {
	PulseAudio pa(argv[0], SAMPLE_RATE, NUM_CHANNELS);

	//Oscillator oscillators[NUM_CHANNELS];
	Synth synth;
	synth.addVoice(69);
	synth.addVoice(69 + 4);
	synth.addVoice(69 + 7);

	int prevNote = 0;

	for (int i = 0; i < 2400; i++) {
		const int note = 69 - 12 + (i / 30);

		if (note != prevNote) {
			std::cout << "Note: " << note << std::endl;
			prevNote = note;
		}

		int16_t buf[BUFSIZE];
		float amplitude = 10000;

		for (unsigned int j = 0; j < BUFSIZE / NUM_CHANNELS; j++) {
			for (unsigned int channel = 0; channel < NUM_CHANNELS; channel++) {
	//			float frequency = noteToFrequency(channel == 0 ? note : note + 7);
	//			float v = oscillators[channel].sine(frequency, SAMPLE_RATE);

	//			int16_t value = clip<int16_t>(v * amplitude);

				float v = synth.mix(SAMPLE_RATE);
				int16_t value = clip<int16_t>(v * amplitude);
				size_t idx = j * NUM_CHANNELS + channel;

				buf[idx] = value;
			}
		}

		pa.write(buf, sizeof(buf));
	}

	pa.drain();

	return 0;
}
