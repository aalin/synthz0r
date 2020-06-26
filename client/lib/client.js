const WebSocket = require('ws');
const EventEmitter = require('events').EventEmitter;

class Client extends EventEmitter {
  constructor(protocol, host, options = {}) {
    super();

    this._log = options.log ? console.log : ()=>{};
    delete options.log;

    this.protocol = protocol;

    this._idCounter = 0;

    this._ws = new WebSocket(host, options);

    this._ws.on('open', () => this.emit('open'));
    this._ws.on('close', () => this.emit('close'));
    this._ws.on('message', (message) => this._handleMessage(message));

    this._callbacks = new Map();
  }

  request(type, data = {}) {
    const id = this._idCounter++;

    const encoded = this.protocol.encode(id, type, data);

    return new Promise((resolve) => {
      this._log('Requesting', id, type, data);
      this._ws.send(encoded);
      this._callbacks.set(id, resolve);
    });
  }

  _handleMessage(encodedMessage) {
    const {id, type, message} = this.protocol.decode(encodedMessage);

    const callback = this._callbacks.get(id);

    if (!callback) {
      console.error(`Could not match ${id} to a promise`);
      return;
    }

    this._log(`Got ${type} for message`, id);

    this._callbacks.delete(id);

    callback(message);
  }
}

module.exports = Client;
