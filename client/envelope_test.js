const path = require('path');
const Protocol = require('./lib/protocol');
const Client = require('./lib/client');
const { notesToArray } = require('./lib/note');

async function main({ port }) {
  const protocol = await Protocol.initialize(path.join(__dirname, 'messages.proto'), 'synthz0r.messages');
  const uri = `ws://localhost:${port}/`;
  const client = new Client(protocol, uri, { log: true });

  console.log('Connecting');

  client.on('open', async () => {
    console.log('connected');

    const createChannelResponse =
      await client.request('CreateChannelRequest', { name: "New channel" });

    const createInstrumentResponse =
      await client.request('CreateDeviceRequest', {
        name: "Drumkit",
        type: client.protocol.enumValues('DeviceType').INSTRUMENT_DEVICE,
        channelId: createChannelResponse.channel.id
      });

    await client.request('UpdateDeviceParametersRequest', {
      id: createInstrumentResponse.device.id,
      parameters: {
        waveformIndex: 4,
        "envelope.attackMs": 500,
        "envelope.decayMs": 500,
        "envelope.sustain": 20,
        "envelope.releaseMs": 500,
      }
    });

    await client.request('PlayRequest');

    let i = 0;

    async function play() {
      const promises = [];

      client.request('NoteOn', {
        channelId: createChannelResponse.channel.id,
        note: 62, // hihat
        velocity: 100,
      });

      if (i % 4 == 0 || i % 8 == 3) {
        client.request('NoteOn', {
          channelId: createChannelResponse.channel.id,
          note: 60, // kick
          velocity: 100,
        });
      } else if (i % 4 == 2) {
        client.request('NoteOn', {
          channelId: createChannelResponse.channel.id,
          note: 61, // snare
          velocity: 100,
        });
      }

      i++;

      setTimeout(play, 300);
    }

    play();
  });
}

main({ port: Number(process.argv[2]) || 5555 });
