const Protocol = require('./protocol');
const Client = require('./client');

async function main({ port }) {
  const protocol = await Protocol.initialize('messages.proto', 'synthz0r.messages');
  const uri = `ws://localhost:${port}/`;
  const client = new Client(protocol, uri);

  client.on('open', () => {
    console.log('connected');

    client.request('ListDevicesRequest').then((response) => {
      console.log('Got response', JSON.stringify(response, null, 2));
    })
  });

  client.on('close', () => {
    console.log('disconnected');
  });

  client.on('message', (data) => {
  });
}

main({ port: Number(process.argv[3]) || 5555 });
