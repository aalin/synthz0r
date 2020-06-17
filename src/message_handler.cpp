#include "message_handler.hpp"
#include "protobuf/messages.pb.h"
#include "message_handler/request.hpp"

namespace messages = synthz0r::messages;

using MessageHandlerNS::Request;

bool createTextResponse(Request &request, std::string message) {
	messages::TextResponse textResponse;
	textResponse.set_message(message);
	return request.setResponse("TextResponse", textResponse);
}

bool createErrorResponse(Request &request, std::string message) {
	messages::ErrorResponse errorResponse;
	errorResponse.set_message(message);
	return request.setResponse("ErrorResponse", errorResponse);
}

bool handleRequest(messages::TextRequest &msg, Request &request, Engine &) {
	std::cout << "textRequest.message() = " << msg.message() << std::endl;

	if (msg.message() == "hello") {
		return createTextResponse(request, "hello world");
	}

	return createErrorResponse(request, "Could not understand whatever you sent");
}

template<typename T>
void setParameters(Devices::DevicePtr device, T *parent) {
	for (const auto &param : device->parameters()) {
		messages::DeviceParameter *p = parent->add_parameters();
		p->set_name(param.name());
		p->set_defaultvalue(param.defaultValue());
		p->set_min(param.min());
		p->set_max(param.max());
		p->set_value(param.value());
	}
}

bool handleRequest(messages::ListDevicesRequest &, Request &request, Engine &engine) {
	std::cout << "Listing devices" << std::endl;

	messages::ListDevicesResponse response;

	for (Devices::DevicePtr device : engine.devices()) {
		messages::Device *d = response.add_devices();
		d->set_id(device->id());
		d->set_name(device->name());

		setParameters(device, d);
	}

	return request.setResponse("ListDevicesResponse", response);
}

bool handleRequest(messages::UpdateDeviceParameterRequest &message, Request &request, Engine &engine) {
	std::cout << "Updating device parameter" << std::endl;

	for (Devices::DevicePtr device : engine.devices()) {
		if (device->id() == message.id()) {
			device->setParam(message.name(), message.value());

			messages::UpdateDeviceParameterResponse response;
			setParameters(device, &response);
			return request.setResponse("UpdateDeviceParameterResponse", response);
		}
	}

	return createErrorResponse(request, "Device not found");
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

#define HANDLER(NAME) {#NAME, parseAndHandle<messages::NAME>}

static const std::map<std::string, std::function<bool(Request &request, Engine &engine)> > Handlers = {
	HANDLER(TextRequest),
	HANDLER(ListDevicesRequest),
	HANDLER(UpdateDeviceParameterRequest),
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
		if (!handler->second(request, engine)) {
			std::cerr << "Could not write response??" << std::endl;
		}
	}

	message->reply(request.encodedResponse());
}
