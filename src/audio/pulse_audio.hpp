#ifndef PULSE_AUDIO_HPP
#define PULSE_AUDIO_HPP

#include <pulse/simple.h>
#include <pulse/error.h>
#include "audio_output.hpp"
#include "sample_format.hpp"

namespace Audio {
	class PulseAudio : public AudioOutput {
		public:
			PulseAudio(
				const char *name,
				SampleFormat::Type sampleFormat,
				unsigned int sampleRate,
				unsigned int channels
			);

			~PulseAudio();

			void write(uint8_t buf[], unsigned int length) { write2(buf, length); }
			void write(int16_t buf[], unsigned int length) { write2(buf, length); }
			void write(int32_t buf[], unsigned int length) { write2(buf, length); }
			void write(float buf[], unsigned int length) { write2(buf, length); }

		private:
			pa_simple *_s = 0;
			pa_sample_spec _ss;
			pa_usec_t _latency;

			void handleError(const char *function, int error, bool throwError = true) const;

			template<typename T>
			void write2(T buf[], unsigned int length) const {
				int error;
				pa_simple_write(_s, (void*)buf, length, &error);
				handleError("pa_simple_write", error);
			}

			void drain();
	};
}

#endif
