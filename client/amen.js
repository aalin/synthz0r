const path = require('path');
const Protocol = require('./lib/protocol');
const Client = require('./lib/client');

const amen = require('./amen.json');

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
        name: "Drumkit",
        type: client.protocol.enumValues('DeviceType').INSTRUMENT_DEVICE,
        channelId: createChannelResponse.channel.id
      });

    const createSequenceResponse = await client.request('CreateSequenceRequest', {
      channelId: createChannelResponse.channel.id,
      start: calculatePosition(0, 0, 0, 0),
      length: calculatePosition(4, 0, 0, 0),
    });

    const sequenceId = createSequenceResponse.sequence.id;

    const promises =
      amen.forEach((event) => (
        client.request('AddSequenceNoteRequest', {
          sequenceId,
          note: event.note,
          velocity: event.velocity,
          start: event.start,
          length: event.end - event.start,
        })
      ));

    //await Promise.all(promises);
    console.log("Setting BPM")

    await client.request('SetBPM', { bpm: 117 });

    await client.request('SetMarkers', {
      left: 0,
      right: calculatePosition(4, 0, 0, 0),
      loop: true
    });

    console.log(await client.request('PlayRequest'))
  });
}

main({ port: Number(process.argv[2]) || 5555 });
