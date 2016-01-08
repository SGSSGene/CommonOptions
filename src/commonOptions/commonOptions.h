#pragma once

#include "Switch.h"
#include "Section.h"
#include "utils.h"

namespace commonOptions {

bool& hasError();
void  print();
void  printShellCompl();
bool  parse(int argc, char const* const* argv);

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

}
