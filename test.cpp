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
}


// Example call1: ./test --help
// Example call2: ./test --str halloWelt
// Example call3: ./test --str=halloWelt
// Example call4: ./test --int 5
int main(int argc, char** args) {
	commonOptions::parse(argc, args);

	std::cout<<*optString<<std::endl;
	std::cout<<*optInt<<std::endl;
	for (auto const& v : *optIntVector) {
		std::cout<<v<<std::endl;
	}
	return 0;
}

