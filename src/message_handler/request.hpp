#ifndef MESSAGE_HANDLER__REQUEST_HPP
#define MESSAGE_HANDLER__REQUEST_HPP

namespace MessageHandlerNS {
	class Request {
		public:
			Request(uint32_t id, std::string encodedRequest)
			: _id(id),
			  _encodedRequest(encodedRequest)
			{}

			std::string encodeResponse(std::unique_ptr<google::protobuf::Message> message) {
				synthz0r::messages::Envelope envelope;
				envelope.set_id(_id);
				envelope.set_type(message->GetTypeName());

				std::string encodedResponse;

				if (!message->SerializeToString(envelope.mutable_payload())) {
					std::cerr << "Could not serialize message" << std::endl;
					throw std::runtime_error("Could not serialize message");
				}

				if (!envelope.SerializeToString(&encodedResponse)) {
					std::cerr << "Could not serialize envelope" << std::endl;
					throw std::runtime_error("Could not serialize envelope");
				}

				return encodedResponse;
			}

			const std::string & encodedRequest() const {
				return _encodedRequest;
			}

		private:
			const uint32_t _id;
			const std::string _encodedRequest;
	};
}

#endif
