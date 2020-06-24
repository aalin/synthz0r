const path = require('path');
const Protocol = require('./protocol');
const Client = require('./client');
const { notesToArray } = require('./note');

async function createSynthWithSequencer(client, deviceName, sequencerData, opts = {}) {
  const createChannelResponse =
    await client.request('CreateChannelRequest', { name: "New channel" });


  const createInstrumentResponse =
    await client.request('CreateDeviceRequest', {
      name: deviceName,
      type: client.protocol.enumValues('DeviceType').INSTRUMENT_DEVICE,
      channelId: createChannelResponse.channel.id
    });

  if (opts.instrumentOpts) {
    client.request('UpdateDeviceParametersRequest', {
      id: createInstrumentResponse.device.id,
      parameters: opts.instrumentOpts
    });
  }

  console.log(JSON.stringify(createInstrumentResponse, null, 2));

  const createSequencerResponse =
    await client.request('CreateDeviceRequest', {
      name: "Sequencer",
      type: client.protocol.enumValues('DeviceType').NOTE_DEVICE,
      channelId: createChannelResponse.channel.id
    });

  if (opts.sequencerOpts) {
    for ([name, value] of Object.entries(opts.sequencerOpts)) {
      client.request('UpdateDeviceParameterRequest', { id: createSequencerResponse.device.id, name, value })
    }
  }

  console.log(JSON.stringify(createSequencerResponse, null, 2));

  const updateSequencerTableResponse =
    await client.request('UpdateDeviceTableRequest', {
      id: createSequencerResponse.device.id,
      name: "notes",
      data: sequencerData
    });

  console.log(updateSequencerTableResponse);
  return createInstrumentResponse.device.id;
}

async function main({ port }) {
  const protocol = await Protocol.initialize(path.join(__dirname, 'messages.proto'), 'synthz0r.messages');
  const uri = `ws://localhost:${port}/`;
  const client = new Client(protocol, uri);

  client.on('open', () => {
    console.log('connected');

    client.request('ListChannelsRequest').then((response) => {
      console.log('Got response', JSON.stringify(response, null, 2));
    });

    /*

    setTimeout(async () => {
      const response = await client.request('TextRequest', { message: "exit" });
      console.log(response);
    }, 5 * 1000);
    */

    createSynthWithSequencer(
      client,
      "WavetableSynth",
      notesToArray(
        "G4 G4 | D5 D5 | E5 E5 | D5 OFF " +
        "C5 C5 | B4 B4 | A4 A4 | G4 OFF " +
        "D5 D5 | C5 C5 | B4 B4 | A4 OFF " +
        "D5 D5 | C5 C5 | B4 B4 | A4 OFF "
      ),
      {
        sequencerOpts: { rate: 0 },
      }
    );

    createSynthWithSequencer(
      client,
      "WavetableSynth",
      notesToArray(
        "G3 G4 G3 G4 | D4 D5 D4 D5 | E4 E5 E4 E5 | D4 D3 D4 D5 " +
        "C4 C5 C4 C5 | B3 B4 B3 B4 | A3 A4 A3 A4 | G3 G2 G3 G4 " +
        "D4 D5 D4 D5 | C4 C5 C4 C5 | B4 B3 B4 B3 | A4 A3 A4 A5 " +
        "D4 D5 D4 D5 | C4 C5 C4 C5 | B4 B3 B4 B3 | A4 A3 A4 A5 "
      ),
      {
        sequencerOpts: { rate: 1 },
        instrumentOpts: {
          waveformIndex: 5,
          transpose: -24,
          "envelope.attackMs": 150,
          "envelope.decayMs": 150,
          "envelope.sustain": 80,
          "envelope.releaseMs": 50,
        }
      }
    )

    createSynthWithSequencer(
      client,
      "Kickdrum",
      notesToArray(
        "C4 C4 C4 C4"
      ),
      { sequencerOpts: { rate: 0 } }
    );

    createSynthWithSequencer(
      client,
      "Synth",
      notesToArray(
        "OFF OFF C4 C4 OFF OFF C4 C4"
      ),
      {
        sequencerOpts: { rate: 2 },
        instrumentOpts: {
          oscillatorType: 4,
          amplitude: 20,
          "envelope.attackMs": 50,
          "envelope.decayMs": 50,
          "envelope.sustain": 30,
          "envelope.releaseMs": 200,
          "filter.type": 1
        }
      }
    ).then((id) => {
      async function updateFilter(deviceId) {
        client.request(
          'UpdateDeviceParametersRequest', {
            id: deviceId,
            parameters: {
              "filter.cutoffHz":
                Math.floor(Math.pow(Math.sin(new Date().getTime() / 5000), 2) * 8000) + 2000,
              "filter.resonance": 10000
            }
          }
        )

        setTimeout(() => updateFilter(deviceId), 100)
      }

      updateFilter(id);
    })
  });

  client.on('close', () => {
    console.log('disconnected');
  });

  client.on('message', (data) => {
  });
}

main({ port: Number(process.argv[2]) || 5555 });
