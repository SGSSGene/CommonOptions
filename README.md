# CommonOptions

Parsing of (simple) command line options

## Defining Parameters
```c
namespace {
	commonOptions::Option<std::string> optString("str", "default", "no description");
	commonOptions::Option<int>         optInt("int", 5, "no description");
	commonOptions::Switch              swtHelp("help", "no description", [](){
		commonOptions::print();
		exit(0);
	});
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

	std::cout<<*optString<<std::endl;
	std::cout<<*optInt<<std::endl;
	return 0;
}
```
