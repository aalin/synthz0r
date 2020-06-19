const Protocol = require('./protocol');
const Client = require('./client');

async function createSynth(client, deviceName) {
  const createChannelResponse =
    await client.request('CreateChannelRequest', { name: "New channel" });

  console.log(createChannelResponse);

  const createInstrumentResponse =
    await client.request('CreateInstrumentDeviceRequest', {
      name: deviceName,
      channelId: createChannelResponse.channel.id
    });

  console.log(JSON.stringify(createInstrumentResponse, null, 2));

  const createSequencerResponse =
    await client.request('CreateNoteDeviceRequest', {
      name: "Sequencer",
      channelId: createChannelResponse.channel.id
    });

  console.log(JSON.stringify(createSequencerResponse, null, 2));

  const updateSequencerParamResponse =
    await client.request('UpdateDeviceParameterRequest', {
      id: createSequencerResponse.device.id,
      name: "bpm",
      value: 120,
    });

  const updateSequencerTableResponse =
    await client.request('UpdateDeviceTableRequest', {
      id: createSequencerResponse.device.id,
      name: "notes",
      data: [69, 70, 71, -1]
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
    })

    createSynth(client, "WavetableSynth");
  });

  client.on('close', () => {
    console.log('disconnected');
  });

  client.on('message', (data) => {
  });
}

main({ port: Number(process.argv[2]) || 5555 });
