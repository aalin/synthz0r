# synthz0r

Simple synth engine. The goal is to have something that is easily controlled via Websockets, so that UIs can be made for browsers, or whatever that supports it.

## Dependencies

* gcc 9.3.0 (but older versions may work too)
* PulseAudio
* websocketpp
  * (depends on boost)
* protobuf

## Building

    make

## Todo

* MIDI-messages
* Websocket API for creating/connecting/controlling devices
* Filters (hipass, lowpass, bandpass)
* More effects
* More instruments
* More controllers (sequencers etc)
