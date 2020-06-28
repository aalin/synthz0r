#include <iostream>
#include "pulse_audio.hpp"

pa_sample_format getPASampleFormat(SampleFormat::Type sampleFormat) {
	switch (sampleFormat) {
		case SampleFormat::Type::UINT8:
			return PA_SAMPLE_U8;
		case SampleFormat::Type::INT16:
			return PA_SAMPLE_S16NE;
		case SampleFormat::Type::INT32:
			return PA_SAMPLE_S32NE;
		case SampleFormat::Type::FLOAT32:
			return PA_SAMPLE_FLOAT32NE;
		default:
			return PA_SAMPLE_INVALID;
	}
}

PulseAudio::PulseAudio(
	const char *name,
	SampleFormat::Type sampleFormat,
	unsigned int sampleRate,
	unsigned int channels
) {
	_ss.format = getPASampleFormat(sampleFormat);
	_ss.rate = sampleRate;
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
		throw std::runtime_error("Could not initialize PulseAudio");
	}

	handleError("pa_simple_new", error);

	_latency = pa_simple_get_latency(_s, &error);

	handleError("pa_simple_get_latency", error);

	std::cout << "Latency: " << (float)_latency << std::endl;
}

void PulseAudio::drain() {
	int error;
	pa_simple_drain(_s, &error);
	handleError("pa_simple_drain", error, false);
}

PulseAudio::~PulseAudio() {
	std::cout << "Destroying PulseAudio" << std::endl;

	drain();

	if (_s != 0) {
		pa_simple_free(_s);
	}
}

void PulseAudio::handleError(const char *function, int error, bool throwError) const {
	if (error >= 0) {
		return;
	}

	std::cerr << function << " failed: " << pa_strerror(error) << std::endl;

	if (throwError) {
		throw std::runtime_error("PulseAudio failed");
	}
}
