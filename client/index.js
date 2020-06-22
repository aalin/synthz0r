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
    for ([name, value] of Object.entries(opts.instrumentOpts)) {
      client.request('UpdateDeviceParameterRequest', { id: createInstrumentResponse.device.id, name, value })
    }
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
}

async function main({ port }) {
  const protocol = await Protocol.initialize('messages.proto', 'synthz0r.messages');
  const uri = `ws://localhost:${port}/`;
  const client = new Client(protocol, uri);

  client.on('open', () => {
    console.log('connected');

    client.request('ListChannelsRequest').then((response) => {
      console.log('Got response', JSON.stringify(response, null, 2));
    });

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
        "G4 G5 G4 G5 | D5 D6 D5 D6 | E5 E6 E5 E6 | D5 D6 OFF OFF " +
        "C5 C6 C5 C6 | B4 B5 B4 B5 | A4 A5 A4 A5 | G4 G5 OFF OFF " +
        "D5 D6 D5 D6 | C5 C6 C5 C6 | B4 B5 B4 B5 | A4 A5 OFF OFF " +
        "D5 D6 D5 D6 | C5 C6 C5 C6 | B4 B5 B4 B5 | A4 A5 OFF OFF "
      ),
      {
        sequencerOpts: { rate: 1 },
        instrumentOpts: {
          waveformIndex: 5,
          transpose: -12
        }
      }
    )

    setTimeout(async () => {
      const response = await client.request('TextRequest', { message: "exit" });
      console.log(response);
    }, 60 * 1000);

    createSynthWithSequencer(
      client,
      "Kickdrum",
      notesToArray(
        "C4 C4 C4 C4 OFF OFF OFF OFF"
      ),
      { sequencerOpts: { rate: 0 } }
    );

    createSynthWithSequencer(
      client,
      "Kickdrum",
      notesToArray(
        "OFF OFF OFF OFF OFF OFF OFF OFF " +
        "C4  OFF C4  OFF C4  OFF C4  OFF "
      ),
      { sequencerOpts: { rate: 1 } }
    );
  });

  client.on('close', () => {
    console.log('disconnected');
  });

  client.on('message', (data) => {
  });
}

main({ port: Number(process.argv[2]) || 5555 });
