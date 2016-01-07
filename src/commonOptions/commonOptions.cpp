#include "commonOptions.h"

namespace commonOptions {

bool& hasError() {
	static bool error{false};
	return error;
}


void print() {
	for (auto f : AllOptions::printMap()) {
		f.second();
	}
}
void printShellCompl() {
	for (auto f : AllOptions::printShellComplMap()) {
		f.second();
	}
}

/**
 * This is a very simple command line parser
 * It can parse stuff like
 * --option
 * --option value
 * --opiton=value
 *
 *  it will ignore everything else on the command line. This is needed because boost::
 *  program_options is handling unknown options very badly
 */
bool parse(int argc, char const* const* argv) {
	std::map<std::string, std::string> options;

	for (int i(0); i<argc; ++i) {
		std::string arg = argv[i];
		if (0 == arg.compare(0, 2, "--")) {
			arg = arg.substr(2); // cut of first two symbols
			std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);

			size_t equalSignPos = arg.find("=");
			if (equalSignPos != std::string::npos) {
				std::string key   = arg.substr(0, equalSignPos);
				std::string value = arg.substr(equalSignPos+1);

				if (AllOptions::baseOptionMap().find(key) == AllOptions::baseOptionMap().end()) {
					hasError() = true;
					continue;
				}
				auto baseOption = AllOptions::baseOptionMap().at(key);
				baseOption->getPreParseFunction()();
				if (not baseOption->getParseFunction()(value)) {
					hasError() = true;
				}
				baseOption->getPostParseFunction()();
			} else if (AllOptions::baseOptionMap().find(arg) == AllOptions::baseOptionMap().end()) {
				hasError() = true;
			} else if (AllOptions::baseOptionMap().at(arg)->getParaType() == ParaType::Multi) {
				std::string key   = arg.substr(0, equalSignPos);
				auto baseOption = AllOptions::baseOptionMap().at(key);

				baseOption->getPreParseFunction()();
				while (i+1 < argc && std::string(argv[i+1]).compare(0, 2, "--") != 0) {
					std::string value = argv[i+1];
					if (not baseOption->getParseFunction()(value)) {
						hasError() = true;
					}
					++i;
				}
				baseOption->getPostParseFunction()();
			} else if (i+1 < argc
					   && std::string(argv[i+1]).compare(0, 2, "--") != 0) {
				std::string key   = arg;
				auto baseOption = AllOptions::baseOptionMap().at(key);

				if (AllOptions::baseOptionMap().at(key)->getParaType() == ParaType::One) {
					std::string value = argv[i+1];


					baseOption->getPreParseFunction()();
					if (not baseOption->getParseFunction()(value)) {
						hasError() = true;
					}
					baseOption->getPostParseFunction()();
					++i;
				} else {
					if (not baseOption->getParseFunction()("1")) {
						hasError() = true;
					}
				}
			} else {
				std::string key = arg;
				auto baseOption = AllOptions::baseOptionMap().at(key);
				baseOption->getPreParseFunction()();
				if (not baseOption->getParseFunction()("1")) {
					hasError() = true;
				}
				baseOption->getPostParseFunction()();
			}
		}
	}
	if (argc == 2 && std::string(argv[1]) == "__completion") {
		printShellCompl();
		exit(0);
	}

	return not hasError();
}

Section* get_section(std::string const& _str) {
	auto path = splitPath(_str);
	Section* section = getRootSection();
	for (auto const& p : path) {
		section = section->accessChild(p);
	}
	return section;
}


Section* getRootSection() {
	static Section singleton;
	return &singleton;
}


}
