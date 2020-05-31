#include <iostream>
#include "pulse_audio.hpp"

PulseAudio::PulseAudio(pa_sample_format_t sampleFormat, const char *name, unsigned int rate, unsigned int channels) : _s(0) {
	_ss.format = sampleFormat;
	_ss.rate = rate;
	_ss.channels = channels;

	int error = 0;

	_s = pa_simple_new(
		0, // default server
		name, // app name
		PA_STREAM_PLAYBACK,
		0, // default device
		"playback", // description of stream
		&_ss, // sample format
		0, // default channel map
		0, // default buffering attributes
		&error
	);

	if (_s == 0) {
		throw "Could not initialize PulseAudio";
	}

	handleError("pa_simple_new", error);

	_latency = pa_simple_get_latency(_s, &error);

	handleError("pa_simple_get_latency", error);

	std::cout << "Latency: " << (float)_latency << std::endl;
}

void PulseAudio::drain() {
	int error;
	pa_simple_drain(_s, &error);
	handleError("pa_simple_drain", error);
}

PulseAudio::~PulseAudio() {
	std::cout << "Destroying PulseAudio" << std::endl;

	if (_s != 0) {
		pa_simple_free(_s);
	}
}

void PulseAudio::handleError(const char *function, int error) const {
	if (error >= 0) {
		return;
	}

	std::cerr << function << " failed: " << pa_strerror(error) << std::endl;
	throw;
}
