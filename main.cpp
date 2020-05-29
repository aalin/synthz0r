#include <iostream>
#include <cmath>
#include <limits>
#include "pulse_audio.hpp"

constexpr unsigned int BUFSIZE = 1024;
constexpr unsigned int SAMPLE_RATE = 44100;
constexpr unsigned int NUM_CHANNELS = 2;

float noteToFrequency(float note) {
	return std::pow(2, (note - 69) / 12) * 440.0;
}

template<typename T, typename X> T clamp(X value) {
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

class Oscillator {
	public:
		Oscillator() : _phase(0) {}

		float sine(float frequency, float sampleRate) {
			constexpr float twoPI = 2.0 * M_PI;

			_phase += twoPI * frequency / sampleRate;

			while(_phase >= twoPI) {
				_phase -= twoPI;
			}

			while(_phase < 0.0) {
				_phase += twoPI;
			}

			return std::sin(_phase);
		}

	private:
		float _phase;
};

int main(int argc, char *argv[]) {
	PulseAudio pa(argv[0], SAMPLE_RATE, NUM_CHANNELS);

	int prevNote = 0;

	// Oscillator oscillators[NUM_CHANNELS];

	for (int i = 0; i < 2400; i++) {
		const int note = 69 - 12 + (i / 10);

		if (note != prevNote) {
			std::cout << "Note: " << note << std::endl;
			prevNote = note;
		}

		int16_t buf[BUFSIZE];
		float amplitude = 10000;

		for (unsigned int j = 0; j < BUFSIZE / NUM_CHANNELS; j++) {
			const float time = (i * BUFSIZE / NUM_CHANNELS + j);
			const float m = M_PI * 2.0 * time / SAMPLE_RATE;

			for (unsigned int channel = 0; channel < NUM_CHANNELS; channel++) {
				float frequency = noteToFrequency(note);
				float theta = frequency * m;

	//			auto &oscillator = oscillators[channel];
	//			auto v = oscillator.sine(frequency, SAMPLE_RATE);
				int16_t value = clamp<int16_t>(std::sin(theta) * amplitude);
				//int16_t value = clamp<int16_t>(v * amplitude);
				//std::cout << idx << " " << th * amplitude << " clamped to " << value << std::endl;
				//int16_t value = 0;

				size_t idx = j * NUM_CHANNELS + channel;
				buf[idx] = value;
			}
		}

		std::cout << "sizeof(buf) " << sizeof(buf) << std::endl;
		pa.write(buf, sizeof(buf));
	}

	pa.drain();

	return 0;
}
