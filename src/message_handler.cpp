#include "message_handler.hpp"
#include "protobuf/messages.pb.h"
#include "message_handler/request.hpp"

namespace messages = synthz0r::messages;

using MessageHandlerNS::Request;

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
	T message;

	if (!message.ParseFromString(request.encodedRequest())) {
		std::cerr << "Could not parse message" << std::endl;
		return false;
	}

	return handleRequest(message, request, engine);
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
