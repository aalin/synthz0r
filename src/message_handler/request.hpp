#ifndef MESSAGE_HANDLER__REQUEST_HPP
#define MESSAGE_HANDLER__REQUEST_HPP

namespace MessageHandlerNS {
	class Request {
		public:
			Request(uint32_t id, std::string encodedRequest)
			: _id(id),
			  _encodedRequest(encodedRequest)
			{}

			template<typename T>
			bool setResponse(const std::string type, const T &message) {
				synthz0r::messages::Envelope envelope;
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
}

#endif
