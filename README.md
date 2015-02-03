# CommonOptions

Parsing of (simple) command line options

## Defining Parameters
```c
namespace {
	commonOptions::Option<std::string> optString("str");
	commonOptions::Option<int>         optInt("int");
	commonOptions::Switch              swtHelp("help");
}
```

## How to use
```c
// Example call1: ./test --help
// Example call2: ./test --str halloWelt
// Example call3: ./test --str=halloWelt
// Example call4: ./test --int 5
int main(int argc, char** args) {
	commonOptions::parse(argc, args);

	if (swtHelp) {
		commonOptions::print();
	}

	std::cout<<*optString<<std::endl;
	std::cout<<*optInt<<std::endl;
	return 0;
}
```
