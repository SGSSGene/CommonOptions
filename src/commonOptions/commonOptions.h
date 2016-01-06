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

template<typename T>
Option<T> make_option(std::string const& _str, T _default, std::string const& _description) {
	return Option<T>(_str, _default, _description);
}
inline Option<std::string> make_option(std::string const& _str, char const* _default, std::string const& _description) {
	return make_option<std::string>(_str, _default, _description);
}

template<typename T>
Option<T> make_option(std::string const& _str, T const& _default, std::set<T> const& _selection, std::string const& _description) {
	return Option<T>(_str, _default, _selection, _description);
}
inline Option<std::string> make_option(std::string const& _str, char const* _default, std::set<char const*> const& _selection, std::string const& _description) {
	std::set<std::string> selection;
	for (auto c : _selection) {
		selection.insert(c);
	}
	return make_option<std::string>(_str, std::string(_default), selection, _description);
}


template<typename T>
Option<std::vector<T>> make_multi_option(std::string const& _str, std::vector<T> const& _selection, std::string const& _description) {
	return Option<std::vector<T>>(_str, _selection, _description);
}
inline Option<std::vector<std::string>> make_multi_option(std::string const& _str, std::vector<char const*> const& _selection, std::string const& _description) {
	std::vector<std::string> selection;
	for (auto c : _selection) {
		selection.push_back(c);
	}
	return make_multi_option<std::string>(_str, selection, _description);
}



inline Switch make_switch(std::string const& _str, std::string const& _description) {
	return Switch(_str, _description);
}
inline Switch make_switch(std::string const& _str, std::string const& _description, std::function<void()> const& _func) {
	return Switch(_str, _description, _func);
}

}
