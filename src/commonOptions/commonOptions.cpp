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

				if (AllOptions::parseParaMap().find(key) == AllOptions::parseParaMap().end()) {
					hasError() = true;
					continue;
				}
				AllOptions::preParseMap()[key]();
				if (not AllOptions::parseMap()[key](value)) {
					hasError() = true;
				}
				AllOptions::postParseMap()[key]();
			} else if (AllOptions::parseParaMap().find(arg) == AllOptions::parseParaMap().end()) {
				hasError() = true;
			} else if (AllOptions::parseParaMap().at(arg) == ParaType::Multi) {
				std::string key   = arg.substr(0, equalSignPos);
				AllOptions::preParseMap()[key]();
				while (i+1 < argc && std::string(argv[i+1]).compare(0, 2, "--") != 0) {
					std::string value = argv[i+1];
					if (not AllOptions::parseMap()[key](value)) {
						hasError() = true;
					}
					++i;
				}
				AllOptions::postParseMap()[key]();
			} else if (i+1 < argc
					   && std::string(argv[i+1]).compare(0, 2, "--") != 0) {
				std::string key   = arg;

				if (AllOptions::parseParaMap().at(key) == ParaType::One) {
					std::string value = argv[i+1];
					AllOptions::preParseMap()[key]();
					if (not AllOptions::parseMap()[key](value)) {
						hasError() = true;
					}
					AllOptions::postParseMap()[key]();
					++i;
				} else {
					if (not AllOptions::parseMap()[key]("1")) {
						hasError() = true;
					}
				}
			} else {
				AllOptions::preParseMap()[arg]();
				if (not AllOptions::parseMap()[arg]("1")) {
					hasError() = true;
				}
				AllOptions::postParseMap()[arg]();

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
	std::cout<<_str<<std::endl;
	for (auto const& p : path) {
		section = section->accessChild(p);
		std::cout << p << std::endl;
	}
	std::cout<<section->fullName() << std::endl;
	return section;
}
Section* getRootSection() {
	static Section singleton;
	return &singleton;
}


}
