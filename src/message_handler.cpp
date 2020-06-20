#include "message_handler.hpp"
#include "protobuf/messages.pb.h"
#include "message_handler/request.hpp"
#include "devices/factory.hpp"
#include "performance_log.hpp"

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
void setParameters(T *parent, Devices::DevicePtr device) {
	for (const auto &param : device->parameters()) {
		messages::DeviceParameter *p = parent->add_parameters();
		p->set_name(param.name());
		p->set_defaultvalue(param.defaultValue());
		p->set_min(param.min());
		p->set_max(param.max());
		p->set_value(param.value());
	}
}

template<typename T>
void setTables(T *parent, Devices::DevicePtr device) {
	for (const auto &table : device->tables()) {
		messages::DeviceTable *p = parent->add_tables();
		p->set_name(table.name());
		p->set_defaultvalue(table.defaultValue());
		p->set_min(table.min());
		p->set_max(table.max());

		google::protobuf::RepeatedField<int> data(table.data().begin(), table.data().end());
		p->mutable_data()->Swap(&data);
	}
}

synthz0r::messages::DeviceType getDeviceType(const Devices::DevicePtr device) {
	if (std::dynamic_pointer_cast<Devices::InstrumentDevice>(device)) {
		return synthz0r::messages::DeviceType::INSTRUMENT_DEVICE;
	}

	if (std::dynamic_pointer_cast<Devices::EffectDevice>(device)) {
		return synthz0r::messages::DeviceType::EFFECT_DEVICE;
	}

	if (std::dynamic_pointer_cast<Devices::NoteDevice>(device)) {
		return synthz0r::messages::DeviceType::NOTE_DEVICE;
	}

	throw std::runtime_error("Could not get device type");
}

void setDevice(messages::Device *msg, const Devices::DevicePtr device) {
	msg->set_id(device->id());
	msg->set_name(device->name());
	std::cout << "Setting device type: " << getDeviceType(device) << std::endl;
	msg->set_type(getDeviceType(device));

	setParameters(msg, device);
	setTables(msg, device);
}

void setChannel(ChannelPtr channel, messages::Channel *msg) {
	msg->set_id(channel->id());
	msg->set_name(channel->name());

	if (channel->getInstrument() != nullptr) {
		setDevice(msg->mutable_instrument(), channel->getInstrument());
	}

	for (const auto effect : channel->getEffectDevices()) {
		setDevice(msg->add_effectdevices(), effect);
	}

	for (const auto noteDevice : channel->getNoteDevices()) {
		setDevice(msg->add_notedevices(), noteDevice);
	}
}

ProtobufMessagePtr handleRequest(messages::ListChannelsRequest &, Engine &engine) {
	std::cout << "Listing channels" << std::endl;

	auto response = std::make_unique<messages::ListChannelsResponse>();

	for (ChannelPtr channel : engine.channels()) {
		setChannel(channel, response->add_channels());
	}

	return response;
}

ProtobufMessagePtr handleRequest(messages::CreateChannelRequest &msg, Engine &engine) {
	auto channel = engine.createChannel(msg.name());

	auto response = std::make_unique<messages::CreateChannelResponse>();

	setChannel(channel, response->mutable_channel());

	return response;
}

ProtobufMessagePtr handleRequest(messages::RemoveChannelRequest &msg, Engine &engine) {
	if (engine.removeChannel(msg.id())) {
		return std::make_unique<messages::SuccessResponse>();
	}

	return createErrorResponse("Could not find channel");
}

Devices::DevicePtr createDeviceFromType(synthz0r::messages::DeviceType type, std::string name) {
	switch (type) {
		case synthz0r::messages::DeviceType::NOTE_DEVICE:
			return Devices::Factory::createNoteDevice(name);
		case synthz0r::messages::DeviceType::INSTRUMENT_DEVICE:
			return Devices::Factory::createInstrumentDevice(name);
		case synthz0r::messages::DeviceType::EFFECT_DEVICE:
			return Devices::Factory::createEffectDevice(name);
		default:
			return nullptr;
	}
}

ProtobufMessagePtr handleRequest(messages::CreateDeviceRequest &message, Engine &engine) {
	auto channel = engine.getChannelById(message.channelid());

	if (channel == nullptr) {
		return createErrorResponse("No such channel");
	}

	auto device = createDeviceFromType(message.type(), message.name());

	if (device == nullptr) {
		return createErrorResponse("Invalid device name/type");
	}

	engine.registerDevice(device);

	switch (message.type()) {
		case synthz0r::messages::DeviceType::NOTE_DEVICE:
			channel->appendNoteDevice(std::dynamic_pointer_cast<Devices::NoteDevice>(device));
			break;
		case synthz0r::messages::DeviceType::INSTRUMENT_DEVICE:
			channel->setInstrument(std::dynamic_pointer_cast<Devices::InstrumentDevice>(device));
			break;
		case synthz0r::messages::DeviceType::EFFECT_DEVICE:
			channel->appendEffectDevice(std::dynamic_pointer_cast<Devices::EffectDevice>(device));
			break;
		default:
			throw std::runtime_error("Invalid device type?? This should never happen.");
	}

	auto response = std::make_unique<messages::CreateDeviceResponse>();
	response->set_channelid(message.channelid());
	setDevice(response->mutable_device(), device);
	return response;
}

ProtobufMessagePtr handleRequest(messages::UpdateDeviceParameterRequest &message, Engine &engine) {
	std::cout << "Updating device parameter" << std::endl;

	auto device = engine.findDeviceById(message.id());

	if (device == nullptr) {
		return createErrorResponse("Device not found");
	}

	device->setParam(message.name(), message.value());

	auto response = std::make_unique<messages::UpdateDeviceParameterResponse>();
	setParameters(response.get(), device);
	return response;
}

ProtobufMessagePtr handleRequest(messages::UpdateDeviceTableRequest &message, Engine &engine) {
	std::cout << "Updating device table" << std::endl;

	auto device = engine.findDeviceById(message.id());

	if (device == nullptr) {
		return createErrorResponse("Device not found");
	}

	std::vector<int> data(message.data().begin(), message.data().end());

	device->setTable(message.name(), data);

	auto response = std::make_unique<messages::UpdateDeviceTableResponse>();
	setTables(response.get(), device);
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
	HANDLER(ListChannelsRequest),
	HANDLER(CreateChannelRequest),
	HANDLER(RemoveChannelRequest),
	HANDLER(CreateDeviceRequest),
	HANDLER(UpdateDeviceParameterRequest),
	HANDLER(UpdateDeviceTableRequest),
};

void MessageHandler::handleMessage(Engine &engine, Websocket::MessagePtr message) {
	PerformanceLog perf;
	messages::Envelope envelope;

	if (!envelope.ParseFromString(message->payload)) {
		message->reply(":::error:::broken envelope");
		return;
	}

	Request request(envelope.id(), envelope.payload());

	perf.log("Parsed request");

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

	perf.log("Created response");

	message->reply(request.encodeResponse(std::move(response)));

	perf.log("Sent reply");
}
