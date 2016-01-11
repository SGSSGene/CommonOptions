#pragma once

#include "Switch.h"
#include "Section.h"
#include "utils.h"

namespace commonOptions {

bool& hasError();
void  print();
void  printShellCompl();

bool parse(int argc, char const* const* argv);

void loadFile(std::string const& _file);
void saveFile(std::string const& _file, std::vector<std::string> const& _includingSections);

Section* get_section(std::string const& _str);

Section* getRootSection();

template <typename ...Args>
auto make_option(Args const&... args) -> decltype(getRootSection()->make_option(args...)) {
	return getRootSection()->make_option(args...);
}

inline auto make_switch(std::string const& _str, std::string const& _description) -> Switch& {
	return getRootSection()->make_switch(_str, _description);
}

inline auto get_option(std::string const& _str) -> BaseOption* {
	return getRootSection()->getVariable(_str);
}
inline auto get_description(std::string const& _str) -> OptionDescription* {
	return getRootSection()->getDescription(_str);
}

}
