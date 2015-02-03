# CommonOptions

Parsing of (simple) command line options

## Defining Parameters

namespace {
	commonOptions::Option<std::string> optString("strpara", "default", "some description");
	commonOptions::Option<int>         optInt("intpara", 0, "some description");
	commonOptions::Switch<bool>        swtHelp("help");
}

## How to use

int main(int argc, char** args) {
	commonOptions::parse(argc, args);

	if (*swtHelp) {
		commonOptions::print();
	}
}

