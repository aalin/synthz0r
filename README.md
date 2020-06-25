# synthz0r

Simple synth engine. The goal is to have something that is easily controlled via Websockets, so that UIs can be made for browsers, or whatever that supports it.

Check out [synthz0r-web-ui](https://github.com/aalin/synthz0r-web-ui) for a web based UI.

## Dependencies

* gcc (9.3.0)
* PulseAudio (13.0)
* websocketpp (0.8.2)
  * (depends on boost (1.72.0))
* protobuf (3.11.4)

## Building

    make

## Usage

Start the server (defaults to port 5555)

    ./synthz0r

Start the client

    node client/index.js 5555

## Todo

* MIDI-messages
* More instruments
* More effects
* More filters
* More controllers (sequencers etc)

## Contributing

Pull requests are welcome. Please try to follow the existing coding style as much as possible.

## License

MIT
