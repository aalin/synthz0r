#ifndef PA_SAMPLE_FORMAT_HPP
#define PA_SAMPLE_FORMAT_HPP

namespace PA {
	template<typename T>
	struct SampleFormat {
		static pa_sample_format_t format() {
			return PA_SAMPLE_INVALID;
		}
	};

	template<>
	struct SampleFormat<uint8_t> {
		static pa_sample_format_t format() {
			return PA_SAMPLE_U8;
		}
	};

	template<>
	struct SampleFormat<int16_t> {
		static pa_sample_format_t format() {
			return PA_SAMPLE_S16NE;
		}
	};

	template<>
	struct SampleFormat<int32_t> {
		static pa_sample_format_t format() {
			return PA_SAMPLE_S32NE;
		}
	};

	template<>
	struct SampleFormat<float> {
		static pa_sample_format_t format() {
			return PA_SAMPLE_FLOAT32NE;
		}
	};
};

#endif
