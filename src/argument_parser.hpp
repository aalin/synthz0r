#ifndef ARGUMENT_PARSER_HPP
#define ARGUMENT_PARSER_HPP

#include <vector>
#include <string>
#include <algorithm>

class ArgumentParser {
	public:
		struct Option {
			Option(std::string option, std::string value, bool found = true)
			: found(found),
			  option(option),
			  value(value)
			{}

			const bool found;
			const std::string option;
			const std::string value;
		};


		ArgumentParser(int argc, char *argv[]) {
			for (int i = 0; i < argc; i++) {
				_args.push_back(argv[i]);
			}
		}

		const Option get(const std::string &option) const {
			auto it = std::find(_args.begin(), _args.end(), option);

			if (it == _args.end() || ++it == _args.end()) {
				return Option(option, "", false);
			}

			return Option(option, *it);
		}

		const std::string get(int index) const {
			return _args.at(index);
		}

		bool has(const std::string &option) const {
			auto it = std::find(_args.begin(), _args.end(), option);

			return it != _args.end();
		}

	private:
		std::vector<std::string> _args;
};

#endif
