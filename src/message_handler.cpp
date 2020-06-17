#include "message_handler.hpp"
#include "protobuf/messages.pb.h"

using namespace synthz0r;

class Request {
	public:
		Request(uint32_t id, std::string encodedRequest)
		: _id(id),
		  _encodedRequest(encodedRequest)
		{}

		template<typename T>
		bool setResponse(const std::string type, const T &message) {
			messages::Envelope envelope;
			envelope.set_id(_id);
			envelope.set_type(type);

			if (!message.SerializeToString(envelope.mutable_payload())) {
				std::cerr << "Could not serialize message" << std::endl;
				return false;
			}

			if (!envelope.SerializeToString(&_encodedResponse)) {
				std::cerr << "Could not serialize envelope" << std::endl;
				return false;
			}

			return true;
		}

		const std::string & encodedResponse() {
			return _encodedResponse;
		}

		const std::string & encodedRequest() const {
			return _encodedRequest;
		}

	private:
		const uint32_t _id;
		const std::string _encodedRequest;
		std::string _encodedResponse;
};

bool createTextResponse(Request &request, std::string payload) {
	messages::TextResponse textResponse;

	textResponse.set_message(payload);

	return request.setResponse("TextResponse", textResponse);
}

bool handleRequest(messages::TextRequest &msg, Request &request, Engine &engine) {
	std::cout << "textRequest.message() = " << msg.message() << std::endl;

	if (msg.message() == "hello") {
		return createTextResponse(request, "hello world");
	}

	return createTextResponse(request, "Could not understand whatever you sent");
}

template<typename T>
bool parseAndHandle(Request &request, Engine &engine) {
	T parsed;

	if (!parsed.ParseFromString(request.encodedRequest())) {
		std::cerr << "Could not parse data" << std::endl;
		return false;
	}

	return handleRequest(parsed, request, engine);
}

static const std::map<std::string, std::function<bool(Request &request, Engine &engine)> > Handlers = {
	{"TextRequest", parseAndHandle<messages::TextRequest>}
};

void MessageHandler::handleMessage(Engine &engine, Websocket::MessagePtr message) {
	messages::Envelope envelope;

	if (!envelope.ParseFromString(message->payload)) {
		message->reply(":::error:::broken envelope");
		return;
	}

	Request request(envelope.id(), envelope.payload());

	const auto handler = Handlers.find(envelope.type());

	if (handler == Handlers.end()) {
		createTextResponse(request, "Unhandled message type: " + envelope.type());
	} else {
		handler->second(request, engine);
	}

	message->reply(request.encodedResponse());
}
