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
		return make_option<std::string>(_str, _default, _selection, _description);
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
		return make_option<std::string>(_str, _default, _description);
	}


	template<typename T>
	auto make_multi_option(std::string const& _str, std::vector<T> const& _selection, std::string const& _description) -> Option<std::vector<T>>{
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_multi_option(v.second, _selection, _description);
		}
		return Option<std::vector<T>>(this, v.second, _selection, _description);
	}
	auto make_multi_option(std::string const& _str, std::vector<char const*> const& _selection, std::string const& _description) -> Option<std::vector<std::string>>{
		std::vector<std::string> retList;
		retList.reserve(_selection.size());
		for (auto c : _selection) {
			retList.push_back(c);
		}
		return make_multi_option<std::string>(_str, retList, _description);
	}


	auto make_switch(std::string const& _str, std::string const& _description, std::function<void()> const& _func) -> Switch {
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_switch(v.second, _description, _func);
		}
		return Switch(this, v.second, _description, _func);
	}
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
