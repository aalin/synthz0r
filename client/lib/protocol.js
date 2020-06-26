const protobuf = require('protobufjs');

class Protocol {
  static initialize(filename, namespace) {
    return new Promise((resolve, reject) => {
      protobuf.load(filename, (err, root) => {
        if (err) {
          return reject(err);
        }

        resolve(new Protocol(root, namespace));
      });
    });
  }

  constructor(root, namespace = '') {
    this._root = root;
    this._namespace = namespace.split('.').filter(Boolean);
  }

  encode(id, type, data = {}) {
    const payload = this._encodeMessage(type, data);

    return this._encodeEnvelope(id, type, payload);
  }

  decode(buffer) {
    const Envelope = this.lookupType('Envelope');
    const envelope = Envelope.decode(buffer);

    const Type = this._root.lookupType(envelope.type);

    return {
      id: envelope.id,
      type: envelope.type,
      message: Type.decode(envelope.payload)
    };
  }

  _encodeEnvelope(id, type, payload) {
    const Message = this.lookupType('Envelope');
    const message = Message.create({
      id, type, payload
    });
    return Message.encode(message).finish();
  }

  _encodeMessage(type, data) {
    const Type = this.lookupType(type);
    const error = Type.verify(data);
    if (error) { throw error; }
    const message = Type.create(data);
    return Type.encode(message).finish();
  }

  enumValues(enumName) {
    return this.lookup(enumName).values;
  }

  lookup(name) {
    return this._root.lookup(this._namespace.concat(name).join('.'));
  }

  lookupType(name) {
    return this._root.lookupType(this._namespace.concat(name).join('.'));
  }
}

module.exports = Protocol;

if (process.argv[1] === __filename) {
  async function main() {
    try {
      const protocol = await Protocol.initialize('synthz0r.proto', 'synthz0r');

      const encoded = protocol.encode('TextRequest', { payload: 'Hello world' });
      console.log('Encoded', encoded);
      const decoded = protocol.decode(encoded);
      console.log(decoded);
    } catch (e) {
      console.error(e.message);
      console.error(e.stack);
    }
  }

  main();
}
