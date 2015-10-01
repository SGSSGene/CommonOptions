/** \example src/testHelp/test.cpp
 *
 * This examples shows how to use the help mechanism
 */
#include "commonOptions/commonOptions.h"
#include <iostream>
#include <vector>

namespace {
	auto optPara = commonOptions::make_option("para1", 0, "an option");
	auto swtPara = commonOptions::make_switch("para2", "just a simple switch");
	auto swtHelp = commonOptions::make_switch("help",  "prints this help");
}

int main(int argc, char** argv) {
	/** Parse options, if something goes wrong print help message **/
	if (not commonOptions::parse(argc, argv)) {
		commonOptions::print();
		exit(0);
	}
	if (*swtHelp) {
		commonOptions::print();
		exit(0);
	}

	return 0;
}
