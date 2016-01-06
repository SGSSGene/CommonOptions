#pragma once

#include <string>
#include <map>
#include "Switch.h"
#include "utils.h"

namespace commonOptions {

class Section {
private:
	Section*    mParent;
	std::string mName;
	std::map<std::string, Section> mChildren;

public:
	Section();
	Section(Section* _parent, std::string const& _name);

	Section* accessChild(std::string const& _name);

	std::string fullName() const;

	auto getSectionOfVariable(std::string const& _str) -> std::pair<Section*, std::string> {
		auto path = splitPath(_str);
		auto variableName = path.back();
		path.pop_back();
		Section* section = this;
		for (auto const& p : path) {
			section = section->accessChild(p);
		}
		return {section, variableName};
	}

	template<typename T>
	auto make_option(std::string const& _str, T _default, std::set<T> const& _selection, std::string const& _description) -> Option<T> {
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_option(v.second, _default, _selection, _description);
		}
		return Option<T>(this, v.second, _default, _selection, _description);
	}
	auto make_option(std::string const& _str, char const* _default, std::set<std::string> const& _selection, std::string const& _description) -> Option<std::string> {
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_option(v.second, _default, _selection, _description);
		}
		return Option<std::string>(this, v.second, _default, _selection, _description);
	}


	template<typename T>
	auto make_option(std::string const& _str, T _default, std::string const& _description) -> Option<T> {
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_option(v.second, _default, _description);
		}
		return Option<T>(this, v.second, _default, _description);
	}
	auto make_option(std::string const& _str, char const* _default, std::string const& _description) -> Option<std::string> {
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_option(v.second, _default, _description);
		}
		return Option<std::string>(this, v.second, _default, _description);
	}


/*	template<typename T>
	Option<std::vector<T>> make_multi_option(std::string const& _str, std::vector<T> const& _selection, std::string const& _description) {
		auto path = splitPath(_str);
		auto variableName = path.back();
		path.pop_back();
		Section* section = this;
		for (auto const& p : path) {
			this = &section->accessChild(p);
		}
		if (section != this) {
			return section->make_multi_option(variableName, _selection, _description);
		}
		return Option<std::vector<T>>(fullName() + "." + variableName, _selection, _description);
	}

	inline Switch make_switch(std::string const& _str, std::string const& _description) {
		return Switch(_str, _description);
	}
	inline Switch make_switch(std::string const& _str, std::string const& _description, std::function<void()> const& _func) {
		return Switch(_str, _description, _func);
	}*/

};

}

namespace commonOptions {
	template<typename T>
	std::string Option<T>::getSectionName() const {
		return mSection->fullName();
	}
	template<typename T>
	std::string Option<std::vector<T>>::getSectionName() const {
		return mSection->fullName();
	}
	template<typename T>
	std::string Option<std::set<T>>::getSectionName() const {
		return mSection->fullName();
	}

}
