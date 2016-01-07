#include "commonOptions.h"

namespace commonOptions {

bool& hasError() {
	static bool error{false};
	return error;
}


void print() {
	auto allOptions = getRootSection()->getVariables();
	for (auto b : allOptions) {
		b->print();
	}
}
void printShellCompl() {
	auto allOptions = getRootSection()->getVariables();
	for (auto b : allOptions) {
		b->printShellCompl();
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

				auto baseOption = get_option(key);
				if (not baseOption) {
					hasError() = true;
					continue;
				}
				if (not baseOption->getParseFunction()(value)) {
					hasError() = true;
				}
			} else if (not get_option(arg)) {
				hasError() = true;
			} else if (get_option(arg)->getParaType() == ParaType::Multi) {
				std::string key   = arg.substr(0, equalSignPos);
				auto baseOption = get_option(key);

				while (i+1 < argc && std::string(argv[i+1]).compare(0, 2, "--") != 0) {
					std::string value = argv[i+1];
					if (not baseOption->getParseFunction()(value)) {
						hasError() = true;
					}
					++i;
				}
			} else if (i+1 < argc
					   && std::string(argv[i+1]).compare(0, 2, "--") != 0) {
				std::string key   = arg;
				auto baseOption = get_option(key);

				if (baseOption->getParaType() == ParaType::One) {
					std::string value = argv[i+1];


					if (not baseOption->getParseFunction()(value)) {
						hasError() = true;
					}
					++i;
				} else {
					if (not baseOption->getParseFunction()("1")) {
						hasError() = true;
					}
				}
			} else {
				std::string key = arg;
				auto baseOption = get_option(key);
				if (not baseOption->getParseFunction()("1")) {
					hasError() = true;
				}
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
