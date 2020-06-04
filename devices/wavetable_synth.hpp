#ifndef WAVETABLE_SYNTH_HPP
#define WAVETABLE_SYNTH_HPP

#include "../waveform.hpp"
#include <list>

namespace Devices {
class WavetableSynth : public BaseDevice {
	public:
		struct Voice {
			Voice(size_t waveformIndex, int note, float velocity, float noteOnTime = 0.0)
			: waveform(Waveform::WAVEFORMS.at(waveformIndex)),
			  note(note),
			  velocity(velocity),
			  noteOnTime(noteOnTime),
			  noteOffTime(-1),
			  _index(0.0)
			{}

			const Waveform &waveform;
			int note;
			float velocity;
			float noteOnTime;
			float noteOffTime;

			float update(float freq, const Timer &timer) {
				_index += M_PI * 2.0 * freq / timer.sampleRate();

				if (_index >= 600) {
					_index = 0.0;
				}

				return waveform.getValue(_index);
			}

			private:

			float _index = 0.0;
		};

		WavetableSynth()
		: BaseDevice("Wavetable Synth", {
			Parameter("pitchBendRange", 0, 24, 2, _pitchBendRange),
			Parameter("transpose", -24, 100, 2, _transpose),
			Parameter("panning", -127, 127, 0, _panning),
			Parameter("amplitude", 0, 128, 0, _amplitude),
			Parameter("envelope.attackMs", 0, 1000, 150, _envelope._attackMs),
			Parameter("envelope.decayMs", 0, 1000, 0, _envelope._decayMs),
			Parameter("envelope.sustain", 0, 127, 0, _envelope._sustain),
			Parameter("envelope.releaseMs", 0, 1000, 0, _envelope._releaseMs),
			Parameter("waveformIndex", 0, Waveform::WAVEFORMS.size(), 0, _waveformIndex)
		  })
		{}

		void noteOn(const Timer &timer, int note, float velocity = 2.0) {
			_voices.push_back(Voice(_waveformIndex, note, velocity, timer.seconds()));
		}

		void noteOff(const Timer &timer, int note) {
			for (auto &v : _voices) {
				if (v.note == note && v.noteOffTime < 0.0) {
					v.noteOffTime = timer.seconds();
				}
			}
		}

		const std::string &getWaveformName() {
			return Waveform::WAVEFORMS.at(_waveformIndex).name;
		}

		void update(const Timer &timer, float pitchBend = 0.0) {
			float v = 0.0;

			for (auto it = _voices.begin(); it != _voices.end();) {
				auto &voice = *it;

				if (_envelope.isNoteDone(timer, voice.noteOffTime)) {
					std::cout << "Removing voice" << std::endl;
					it = _voices.erase(it);
					continue;
				}

				constexpr int waveTranspose = 22 + 24 + 12;
				float freq = Utils::noteToFrequency(voice.note + waveTranspose + _transpose + _pitchBendRange * pitchBend);
				float value = voice.update(freq, timer);
				float env = _envelope.update(timer, voice.noteOnTime, voice.noteOffTime);

				v += Utils::volume(value * voice.velocity * env * amplitude());

				++it;
			}

			StereoSample out = Utils::pan(v, panning());
			// std::cout << out<<std::endl;
			output(timer, out);
		}

	private:
		ADSR _envelope;
		int _amplitude;
		int _pitchBendRange;
		int _transpose;
		int _panning;
		int _waveformIndex;

		std::list<Voice> _voices;

		float amplitude() const {
			return _amplitude / 100.0;
		}

		float panning() const {
			return _panning / 127.0;
		}
};
};

#endif
