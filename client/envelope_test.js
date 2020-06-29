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

    let noteOn = false;

    async function play() {
      if (noteOn) {
        console.log('NoteOff');

        await client.request('NoteOff', {
          channelId: createChannelResponse.channel.id,
          note: 60,
        });

        noteOn = false;
      } else {
        console.log('NoteOn');

        await client.request('NoteOn', {
          channelId: createChannelResponse.channel.id,
          note: 60,
          velocity: 100,
        });

        noteOn = true;
      }

      setTimeout(play, 2000);
    }

    play();
  });
}

main({ port: Number(process.argv[2]) || 5555 });
