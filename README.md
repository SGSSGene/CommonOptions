# CommonOptions

Parsing of (simple) command line options

## Defining Parameters
```c
namespace {
	commonOptions::Option<std::string> optString("strpara", "default", "some description");
	commonOptions::Option<int>         optInt("intpara", 0, "some description");
	commonOptions::Switch<bool>        swtHelp("help");
}
```

## How to use
```c
int main(int argc, char** args) {
	commonOptions::parse(argc, args);

	if (*swtHelp) {
		commonOptions::print();
	}
}
```
