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

	std::map<std::string, std::unique_ptr<BaseOption>> mVariables;
public:
	Section();
	Section(Section const& _other);
	Section(Section* _parent, std::string const& _name);

	Section& operator=(Section const& _other);

	std::vector<BaseOption*> getVariables();
private:
	void getVariablesImpl(std::vector<BaseOption*>* options);
public:
	BaseOption* getVariable(std::string const& _name);

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
	auto make_option(std::string const& _str, T _default, std::set<T> const& _selection, std::string const& _description) -> Option<T>& {
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_option(v.second, _default, _selection, _description);
		}
		if (mVariables.find(v.second) == mVariables.end()) {
			mVariables[v.second].reset(new Option<T>(this, v.second, _default, _selection, _description));
		}
		return dynamic_cast<Option<T>&>(*mVariables.at(v.second));
	}
	auto make_option(std::string const& _str, char const* _default, std::set<std::string> const& _selection, std::string const& _description) -> Option<std::string>& {
		return make_option<std::string>(_str, _default, _selection, _description);
	}


	template<typename T>
	auto make_option(std::string const& _str, T _default, std::string const& _description) -> Option<T>& {
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_option(v.second, _default, _description);
		}
		if (mVariables.find(v.second) == mVariables.end()) {
			mVariables[v.second].reset(new Option<T>(this, v.second, _default, _description));
		}
		return dynamic_cast<Option<T>&>(*mVariables.at(v.second));
	}
	auto make_option(std::string const& _str, char const* _default, std::string const& _description) -> Option<std::string>& {
		return make_option<std::string>(_str, _default, _description);
	}

	auto make_switch(std::string const& _str, std::string const& _description) -> Switch& {
		auto v = getSectionOfVariable(_str);
		if (v.first != this) {
			return v.first->make_switch(v.second, _description);
		}
		if (mVariables.find(v.second) == mVariables.end()) {
			mVariables[v.second].reset(new Switch(this, v.second, _description));
		}
		return dynamic_cast<Switch&>(*mVariables.at(v.second));
	}
};

}
