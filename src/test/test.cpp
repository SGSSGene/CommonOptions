#include "commonOptions/commonOptions.h"
#include <iostream>
#include <vector>

namespace {
	auto optString    = commonOptions::make_option("str", "default", "no description");
	auto optInt       = commonOptions::make_option("int", 5,         "no description");
	auto optIntVector = commonOptions::make_option<int>("intVec", {0, 2}, "no description for int Vec");
	auto swtHelp      = commonOptions::make_switch("help", "no description", [] {
		commonOptions::print();
		exit(0);
	});
	auto optStringOpt = commonOptions::make_option<std::string>("strstr", "option1", {"option1", "option2"}, "an option, with 2 possible values");
}


// Example call1: ./test --help
// Example call2: ./test --str halloWelt
// Example call3: ./test --str=halloWelt
// Example call4: ./test --int 5
int main(int argc, char** args) {
	if (not commonOptions::parse(argc, args)) {
		commonOptions::print();
		exit(0);
	}


	std::cout<<*optString<<std::endl;
	std::cout<<*optInt<<std::endl;
	for (auto const& v : *optIntVector) {
		std::cout<<v<<std::endl;
	}
	std::cout<<*optStringOpt<<std::endl;
	return 0;
}

