const Protocol = require('./protocol');
const Client = require('./client');

async function main() {
  const protocol = await Protocol.initialize('messages.proto', 'synthz0r.messages');
  const client = new Client(protocol, 'ws://localhost:5556/');

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

main();
