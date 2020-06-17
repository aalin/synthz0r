#include "message_handler.hpp"
#include "protobuf/messages.pb.h"

using namespace synthz0r;

class Request {
	public:
		Request(uint32_t id, std::string data)
		: _id(id),
		  _data(data)
		{}

		template<typename T>
		T parse() {
			T msg;

			if (!msg.ParseFromString(_data)) {
				throw "Could not parse data";
			}

			return msg;
		}

		template<typename T>
		bool setResponse(const std::string type, const T &message) {
			messages::Envelope envelope;
			envelope.set_id(_id);
			envelope.set_type(type);

			if (!message.SerializeToString(envelope.mutable_payload())) {
				std::cerr << "Could not serialize message" << std::endl;
				return false;
			}

			if (!envelope.SerializeToString(&_response)) {
				std::cerr << "Could not serialize envelope" << std::endl;
				return false;
			}

			return true;
		}

		const std::string & response() {
			return _response;
		}

	private:
		const uint32_t _id;
		const std::string _data;
		std::string _response;
};

bool createTextResponse(Request &request, std::string payload) {
	messages::TextResponse textResponse;

	textResponse.set_message(payload);

	return request.setResponse("TextResponse", textResponse);
}

bool handleTextRequest(Request &request) {
	messages::TextRequest textRequest = request.parse<messages::TextRequest>();

	std::cout << "textRequest.message() = " << textRequest.message() << std::endl;

	if (textRequest.message() == "hello") {
		return createTextResponse(request, "hello world");
	}

	return createTextResponse(request, "Could not understand whatever you sent");
}

void MessageHandler::handleMessage(Engine &engine, Websocket::MessagePtr message) {
	messages::Envelope envelope;

	if (!envelope.ParseFromString(message->payload)) {
		message->reply(":::error:::broken envelope");
		return;
	}

	std::cout << envelope.DebugString() << std::endl;

	Request request(envelope.id(), envelope.payload());

	if (envelope.type() == "TextRequest") {
		handleTextRequest(request);
	} else {
		createTextResponse(request, "Unhandled message type: " + envelope.type());
	}

	message->reply(request.response());
}
