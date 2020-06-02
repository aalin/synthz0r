#ifndef FILE_OUTPUT_HPP
#define FILE_OUTPUT_HPP

#include "audio_output.hpp"
#include "sample_format.hpp"
#include <fstream>

class FileOutput : public AudioOutput {
	struct WavHeader {
		char    riffTag[4] = {'R', 'I', 'F', 'F'};
		int32_t fileSize;
		char    waveTag[4] = {'W', 'A', 'V', 'E'};
		char    fmtTag[4] = {'f', 'm', 't', ' '};
		int32_t fmtLength = 16;
		int16_t audioFormat = 1; // 1 = PCM
		int16_t numChannels = 0;
		int32_t sampleRate = 0;
		int32_t byteRate;
		int16_t blockAlign;
		int16_t bitsPerSample;
		char    dataTag[4] = {'d', 'a', 't', 'a'};
		int32_t dataLength;
	};

	public:
		FileOutput(std::string filename, SampleFormat::Type sampleFormat, unsigned int sampleRate, unsigned int numChannels)
		: _out(filename.c_str(), std::ios::binary)
		{
			writeHeader(sampleFormat, sampleRate, numChannels);
		}

		void write(uint8_t buf[], unsigned int length) { write2(buf, length); }
		void write(int16_t buf[], unsigned int length) { write2(buf, length); }
		void write(int32_t buf[], unsigned int length) { write2(buf, length); }
		void write(float buf[], unsigned int length) { write2(buf, length); }

		void drain() {
			if (_out.bad()) {
				std::cout << "Out is bad" << std::endl;
				return;
			}

			std::cout << "Updating sizes." << std::endl;
			int32_t fileSize = sizeof(WavHeader) + _bytesWritten;
			std::cout << "File size" << fileSize << std::endl;
			_out.seekp(4, std::ios::beg);
			_out.write(reinterpret_cast<const char*>(&fileSize), 4);
			_out.seekp(40, std::ios::beg);
			_out.write(reinterpret_cast<const char*>(&_bytesWritten), 4);
		}

	private:
		std::ofstream _out;
		int32_t _bytesWritten;

		void writeHeader(SampleFormat::Type type, unsigned int sampleRate, unsigned int numChannels) {
			WavHeader header;
			header.fileSize = 0xdeadbeef;
			header.numChannels = numChannels;
			header.sampleRate = sampleRate;
			header.bitsPerSample = SampleFormat::bitsPerSample(type);
			header.byteRate = (sampleRate * header.bitsPerSample * numChannels) / 8;
			header.dataLength = 0xdeadbeef;
			_out.write(reinterpret_cast<const char*>(&header), sizeof(header));
		}

		template<typename T>
		void write2(T buf[], unsigned int length) {
			_out.write(reinterpret_cast<char*>(buf), length);
			_bytesWritten += length;
		}
};

#endif
