#include "message_handler.hpp"
#include "protobuf/messages.pb.h"
#include "message_handler/request.hpp"
#include "device_factory.hpp"

namespace messages = synthz0r::messages;

using MessageHandlerNS::Request;

typedef std::unique_ptr<google::protobuf::Message> ProtobufMessagePtr;

ProtobufMessagePtr createTextResponse(const std::string &message) {
	auto response = std::make_unique<messages::TextResponse>();
	response->set_message(message);
	return response;
}

ProtobufMessagePtr createErrorResponse(const std::string &message) {
	auto response = std::make_unique<messages::ErrorResponse>();
	response->set_message(message);
	return response;
}

ProtobufMessagePtr handleRequest(messages::TextRequest &msg, Engine &) {
	std::cout << msg.GetTypeName() << ".message() = " << msg.message() << std::endl;

	if (msg.message() == "hello") {
		return createTextResponse("hello world");
	}

	return createErrorResponse("Could not understand whatever you sent");
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

void setDevice(Devices::DevicePtr device, messages::Device *ptr) {
	ptr->set_id(device->id());
	ptr->set_name(device->name());

	setParameters(device, ptr);
}

ProtobufMessagePtr handleRequest(messages::ListDevicesRequest &, Engine &engine) {
	std::cout << "Listing devices" << std::endl;

	auto response = std::make_unique<messages::ListDevicesResponse>();

	for (Devices::DevicePtr device : engine.devices()) {
		setDevice(device, response->add_devices());
	}

	return response;
}

ProtobufMessagePtr handleRequest(messages::UpdateDeviceParameterRequest &message, Engine &engine) {
	std::cout << "Updating device parameter" << std::endl;

	for (Devices::DevicePtr device : engine.devices()) {
		if (device->id() == message.id()) {
			device->setParam(message.name(), message.value());

			auto response = std::make_unique<messages::UpdateDeviceParameterResponse>();
			setParameters(device, response.get());
			return response;
		}
	}

	return createErrorResponse("Device not found");
}

ProtobufMessagePtr handleRequest(messages::CreateDeviceRequest &message, Engine &engine) {
	auto device = DeviceFactory::create(message.name());

	if (device == nullptr) {
		return createErrorResponse("Invalid device name");
	}

	engine.addDevice(device);

	auto response = std::make_unique<messages::CreateDeviceResponse>();
	setDevice(device, response->mutable_device());
	return response;
}

ProtobufMessagePtr handleRequest(messages::ConnectDeviceRequest &message, Engine &engine) {
	const uint64_t sourceId = message.sourceid();
	const uint64_t targetId = message.targetid();

	auto source = engine.findDeviceById(sourceId);

	if (source == nullptr) {
		return createErrorResponse("Source not found");
	}

	auto target = engine.findDeviceById(targetId);

	if (target == nullptr) {
		return createErrorResponse("Target not found");
	}

	source->outputs().add(target);

	auto response = std::make_unique<messages::ConnectDeviceResponse>();

	response->mutable_connection()->set_sourceid(source->id());

	for (auto output : source->outputs()) {
		response->mutable_connection()->add_targetids(output->id());
	}

	return response;
}

template<typename T>
ProtobufMessagePtr parseAndHandle(Request &request, Engine &engine) {
	T message;

	if (!message.ParseFromString(request.encodedRequest())) {
		std::cerr << "Could not parse message" << std::endl;
		return createErrorResponse("Could not parse message");
	}

	return handleRequest(message, engine);
}

#define HANDLER(NAME) {#NAME, parseAndHandle<messages::NAME>}

static const std::map<std::string, std::function<ProtobufMessagePtr(Request &request, Engine &engine)> > Handlers = {
	HANDLER(TextRequest),
	HANDLER(ListDevicesRequest),
	HANDLER(UpdateDeviceParameterRequest),
	HANDLER(CreateDeviceRequest),
	HANDLER(ConnectDeviceRequest),
};

void MessageHandler::handleMessage(Engine &engine, Websocket::MessagePtr message) {
	messages::Envelope envelope;

	if (!envelope.ParseFromString(message->payload)) {
		message->reply(":::error:::broken envelope");
		return;
	}

	Request request(envelope.id(), envelope.payload());
	ProtobufMessagePtr response;

	try {
		const auto handler = Handlers.find(envelope.type());

		if (handler == Handlers.end()) {
			response = createErrorResponse("Unhandled message type: " + envelope.type());
		} else {
			response = handler->second(request, engine);

			if (response == nullptr) {
				std::cerr << "Got null response" << std::endl;
				response = createErrorResponse("Got null response");
			}
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		response = createErrorResponse("std::exception occurred");
	} catch (...) {
		response = createErrorResponse("Unknown exception occurred");
	}

	message->reply(request.encodeResponse(std::move(response)));
}
