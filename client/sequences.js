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

function randomRange(min, max) {
  return min + Math.random() * (max - min);
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
        waveformIndex: 3,
        "envelope.attackMs": 20,
        "envelope.decayMs": 100,
        "envelope.sustain": 50,
        "envelope.releaseMs": 1000,
      }
    });

    const createSequenceResponse = await client.request('CreateSequenceRequest', {
      channelId: createChannelResponse.channel.id,
      start: calculatePosition(0, 0, 0, 0),
      length: calculatePosition(4, 0, 0, 0),
    });

    const sequenceId = createSequenceResponse.sequence.id;
    let position = 0;

    const promises = [];

    for (let chord of CHORDS) {
      const length = calculatePosition(0, 2, 0, 0);

      chord.forEach((note, i) => {
        let startOffset = Math.floor(i * 240 / 2);

        let length2 = randomRange(
          calculatePosition(0, 0, 0, 120),
          calculatePosition(0, 0, 1, 0),
        );

        promises.push(client.request('AddSequenceNoteRequest', {
          sequenceId,
          note,
          velocity: 100,
          start: position + startOffset,
          length: Math.floor(length2)
        }))
      });

      position += length;
    }

    await Promise.all(promises);

    client.request('SetBPM', { bpm: 80 });
    client.request('SetMarkers', {
      left: 0,
      right: calculatePosition(4, 0, 0, 0),
      loop: true
    })

    await client.request('PlayRequest');
  });
}

main({ port: Number(process.argv[2]) || 5555 });
