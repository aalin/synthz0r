const path = require('path');
const Protocol = require('./lib/protocol');
const Client = require('./lib/client');

const CHORDS = [
  [ 69, 60, 64 ],
  [ 64, 68, 71, 62 ],
  [ 67, 71, 62 ],
  [ 62, 66, 69 ],
  [ 65, 69, 60 ],
  [ 60, 64, 67 ],
  [ 62, 65, 69 ],
  [ 64, 68, 71 ]
];

function calculatePosition(bars, beats, sixteenths, ticks) {
  return (
    bars * 4 * 4 * 240 +
    beats * 4 * 240 +
    sixteenths * 240 +
    ticks
  );
}

async function main({ port }) {
  const protocol = await Protocol.initialize(path.join(__dirname, 'messages.proto'), 'synthz0r.messages');
  const client = new Client(protocol, `ws://localhost:${port}`, { log: true });

  client.on('open', async () => {
    console.log('connected');

    const createChannelResponse =
      await client.request('CreateChannelRequest', { name: "New channel" });

    const createInstrumentResponse =
      await client.request('CreateDeviceRequest', {
        name: "WavetableSynth",
        type: client.protocol.enumValues('DeviceType').INSTRUMENT_DEVICE,
        channelId: createChannelResponse.channel.id
      });

    await client.request('UpdateDeviceParametersRequest', {
      id: createInstrumentResponse.device.id,
      parameters: {
        waveformIndex: 4,
        "envelope.attackMs": 100,
        "envelope.decayMs": 100,
        "envelope.sustain": 20,
        "envelope.releaseMs": 500,
      }
    });

    const createSequenceResponse = await client.request('CreateSequenceRequest', {
      channelId: createChannelResponse.channel.id,
      start: calculatePosition(0, 0, 0, 0),
      length: calculatePosition(4, 0, 0, 0),
    });

    const sequenceId = createSequenceResponse.sequence.id;
    let position = 0;

    for (let chord of CHORDS) {
      const length = calculatePosition(0, 2, 0, 0);

      for (let note of chord) {
        await client.request('AddSequenceNoteRequest', {
          sequenceId,
          note,
          velocity: 100,
          start: position + Math.floor(Math.random() * 240 * 4),
          length: Math.floor(length * Math.random())
        })
      }

      position += length;
    }

    await client.request('PlayRequest');
  });
}

main({ port: Number(process.argv[2]) || 5555 });
