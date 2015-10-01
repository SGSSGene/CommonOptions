/** \example src/testMultiOption/test.cpp
 *
 * This examples shows how to deal with parameter that can have
 * multiple arguments
 */
#include "commonOptions/commonOptions.h"
#include <iostream>
#include <vector>

namespace {
	auto optPara1 = commonOptions::make_option<int>        ("para1", std::vector<int>{}, "this takes multiple strings as arguments");
	auto optPara2 = commonOptions::make_option<std::string>("para2", {}, "this takes multiple strings as arguments");

}

int main(int argc, char** argv) {
	commonOptions::parse(argc, argv); // parsing options

	// print options
	for (auto const& s : *optPara1) {
		std::cout << "para1: " << s << std::endl;
	}
	for (auto const& s : *optPara2) {
		std::cout << "para2: " << s << std::endl;
	}


	return 0;
}

