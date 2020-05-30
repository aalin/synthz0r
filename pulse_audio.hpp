#ifndef PULSE_AUDIO_HPP
#define PULSE_AUDIO_HPP

#include <pulse/simple.h>
#include <pulse/error.h>
#include "pa_sample_format.hpp"

class PulseAudio {
	public:
		PulseAudio(pa_sample_format_t sampleFormat, const char *name, unsigned int rate, unsigned int channels);

		~PulseAudio();

		template<typename T>
		void write(T buf[], unsigned int length) {
			int error;
			pa_simple_write(_s, (void*)buf, length, &error);
			handleError("pa_simple_write", error);
		}

		void drain();

	private:
		pa_simple *_s;
		pa_sample_spec _ss;
		pa_usec_t _latency;

		void handleError(const char *function, int error);
};

#endif
