const path = require('path');
const Protocol = require('./lib/protocol');
const Client = require('./lib/client');
const { notesToArray } = require('./lib/note');

async function createSynthWithSequencer(client, deviceName, sequencerData, opts = {}) {
  const createChannelResponse =
    await client.request('CreateChannelRequest', { name: "New channel" });


  const createInstrumentResponse =
    await client.request('CreateDeviceRequest', {
      name: deviceName,
      type: client.protocol.enumValues('DeviceType').INSTRUMENT_DEVICE,
      channelId: createChannelResponse.channel.id
    });

  if (opts.instrumentParams) {
    client.request('UpdateDeviceParametersRequest', {
      id: createInstrumentResponse.device.id,
      parameters: opts.instrumentParams
    });
  }

  console.log(JSON.stringify(createInstrumentResponse, null, 2));

  const createSequencerResponse =
    await client.request('CreateDeviceRequest', {
      name: "Sequencer",
      type: client.protocol.enumValues('DeviceType').NOTE_DEVICE,
      channelId: createChannelResponse.channel.id
    });

  if (opts.sequencerParams) {
    client.request('UpdateDeviceParametersRequest', {
      id: createSequencerResponse.device.id,
      parameters: opts.sequencerParams
    });
  }

  console.log(JSON.stringify(createSequencerResponse, null, 2));

  const updateSequencerTableResponse =
    await client.request('UpdateDeviceTablesRequest', {
      id: createSequencerResponse.device.id,
      tables: {
        notes: { values: sequencerData }
      }
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
        sequencerParams: { rate: 0 },
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
        sequencerParams: { rate: 1 },
        instrumentParams: {
          waveformIndex: 4,
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
      {
        sequencerParams: { rate: 0 },
        instrumentParams: { pitch: 880 }
      }
    );

    createSynthWithSequencer(
      client,
      "Synth",
      notesToArray(
        "OFF OFF C4 C4 OFF OFF C4 C4"
      ),
      {
        sequencerParams: { rate: 2 },
        instrumentParams: {
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
      let playing = false;

      async function togglePlayPause() {
        let timeout = 2000;

        if (playing) {
          console.log('Playing');
          await client.request('PlayRequest');
          timeout += Math.random() * 8000;
        } else {
          console.log('Pausing');
          await client.request('PauseRequest');
        }

        playing = !playing;

        setTimeout(togglePlayPause, timeout);
      }

      togglePlayPause();

      async function updateFilter(deviceId) {
        const filterCutoffHz = Math.floor(Math.pow(Math.sin(new Date().getTime() / 500), 2) * 8000) + 2000;

        console.log('Setting filter cutoff', filterCutoffHz);

        client.request(
          'UpdateDeviceParametersRequest', {
            id: deviceId,
            parameters: {
              "filter.cutoffHz": filterCutoffHz,
              "filter.resonance": 10000
            }
          }
        );

        setTimeout(() => updateFilter(deviceId), 100);
      }

      //updateFilter(id);
    })
  });

  client.on('close', () => {
    console.log('disconnected');
  });

  client.on('message', (data) => {
  });
}

main({ port: Number(process.argv[2]) || 5555 });
