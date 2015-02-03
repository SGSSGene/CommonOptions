#include "commonOptions/commonOptions.h"
#include <iostream>

namespace {
	commonOptions::Option<std::string> optString("str");
	commonOptions::Option<int>         optInt("int");
	commonOptions::Switch<bool>        swtHelp("help");
}

int main(int argc, char** args) {
	commonOptions::parse(argc, args);

	if (*swtHelp) {
		commonOptions::print();
	}
	return 0;
}

