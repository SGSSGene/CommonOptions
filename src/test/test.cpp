#include "commonOptions/commonOptions.h"
#include <iostream>
#include <vector>

namespace {
	commonOptions::Option<std::string> optString("str", "default", "no description");
	commonOptions::Option<int>         optInt("int", 5, "no description");
	commonOptions::Option<std::vector<int>> optIntVector("intVec", {0, 2}, "no description for int Vec");
	commonOptions::Switch              swtHelp("help", "no description", []() {
		commonOptions::print();
		exit(0);
	});
	commonOptions::Option<std::string> optStringOpt("strstr", "option1", {"option1", "option2"}, "an option, with 2 possible values");
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

