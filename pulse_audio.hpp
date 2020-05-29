#ifndef PULSE_AUDIO_HPP
#define PULSE_AUDIO_HPP

#include <pulse/simple.h>
#include <pulse/error.h>

class PulseAudio {
	public:
		PulseAudio(const char *name, unsigned int rate, unsigned int channels);
		~PulseAudio();

		template<typename T>
		void write(T buf[], unsigned int length) {
			std::cout << "Writing buffer " << length << " bytes to " << _s << std::endl;
			int error;
			pa_simple_write(_s, (void*)buf, length, &error);
			handleError("pa_simple_write", error);
		}

		void drain();

	private:
		unsigned int _bufferSize;
		pa_simple *_s;
		pa_sample_spec _ss;
		pa_usec_t _latency;

		void handleError(const char *function, int error);
};

#endif
