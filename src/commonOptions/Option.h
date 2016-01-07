#pragma once

#include "AllOptions.h"
#include "BaseOption.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

namespace commonOptions {

class Section;

/**
 * This type must be convertable by using stringstream
 */
template<typename T>
class Option : public BaseOption {
private:
	bool        onlyPossibleValues;
	std::set<T> possibleValues;
	OptionDescription<T> mOptionDescription;
public:
	Option(Section* _section, std::string const& _name, T const& _default, std::string const& _description)
		: Option(_section, _name, _default, {}, _description, [](T const&){}) {
	}
	Option(Section* _section, std::string const& _name, T const& _default, std::set<T> const& _list, std::string const& _description)
		: Option(_section, _name, _default, _list, _description, [](T const&){}) {
	}
	Option(Section* _section, std::string _name, T const& _default, std::string const& _description, std::function<void(T const&)> _func)
		: Option(_section, _name, _default, {}, _description, _func) {
	}


	Option(Section* _section, std::string _varName, T const& _default, std::set<T> const& _list, std::string const& _description, std::function<void(T const&)> _func)
		: BaseOption(_section, _varName, commonOptions::getParaType<T>())
	{
		onlyPossibleValues = not _list.empty();
		possibleValues = _list;
		possibleValues.insert(_default);

		auto _name = getSectionName() + _varName;
		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		mOptionDescription.optionName   = _name;
		mOptionDescription.description  = _description;
		mOptionDescription.defaultValue = _default;
		mOptionDescription.value        = _default;

		mParseFunction = [&](std::string const& _value) {
			std::stringstream ss;
			ss<<_value;
			ss>>mOptionDescription.value;
			if (onlyPossibleValues) {
				bool foundValue {false};
				for (auto const& s : possibleValues) {
					if (mOptionDescription.value == s) {
						foundValue = true;
						break;
					}
				}
				if (not foundValue) {
					std::cerr<<"Wrong option: "<<getName()<<" doesn't accept: "<<_value<<std::endl;
					return false;
				}
			}
			return true;
		};
		AllOptions::printMap()[_name] = [=]() {
			std::stringstream ss;
			ss<<"--"<<mOptionDescription.optionName;
			if (AllOptions::baseOptionMap().at(_name)->getParaType() != ParaType::None) {
				ss<<" "<<mOptionDescription.defaultValue;
			}
			while(ss.str().length() < 32) {
				ss<<" ";
			}
			ss<<mOptionDescription.description;
			std::cout<<ss.str()<<std::endl;
		};
		AllOptions::printShellComplMap()[_name] = [=]() {
			std::cout<<"--"<<mOptionDescription.optionName<<" ";
		};

		mPreParseFunction = [] {};
		mPostParseFunction = [this, _func] { _func(mOptionDescription.value); };
	}

	T const* operator->() const {
		return &mOptionDescription.value;
	}
	T const& operator*() const {
		return mOptionDescription.value;
	}
};
template<typename T>
class Option<std::vector<T>> : public BaseOption {
private:
	OptionDescription<std::vector<T>> mOptionDescription;
public:
	Option(Section* _section, std::string const& _name, std::vector<T> const& _default, std::string const& _description)
		: Option(_section, _name, _default, _description, [](std::vector<T> const&){}) {
	}

	Option(Section* _section, std::string _varName, std::vector<T> const& _default, std::string const& _description, std::function<void(std::vector<T> const&)> _func)
		: BaseOption(_section, _varName, commonOptions::getParaType<std::vector<T>>())
	{
		auto _name = getSectionName() + _varName;

		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		mOptionDescription.optionName   = _name;
		mOptionDescription.description  = _description;
		mOptionDescription.defaultValue = _default;
		mOptionDescription.value        = _default;

		mParseFunction = [&](std::string const& _value) {
			std::stringstream ss;
			ss<<_value;
			T t;
			ss>>t;
			mOptionDescription.value.push_back(t);
			return true;
		};
		AllOptions::printMap()[_name] = [=]() {
			std::stringstream ss;
			ss<<"--"<<mOptionDescription.optionName<<" { ";
			for (auto const& v : mOptionDescription.defaultValue) {
				ss << v << ", ";
			}
			ss<<"}";
			while(ss.str().length() < 32) {
				ss<<" ";
			}
			ss<<mOptionDescription.description;
			std::cout<<ss.str()<<std::endl;
		};
		AllOptions::printShellComplMap()[_name] = [=]() {
			std::cout<<"--"<<mOptionDescription.optionName<<" ";
		};
		mPreParseFunction  = [&]() { mOptionDescription.value.clear(); };
		mPostParseFunction = [this, _func]() { _func(mOptionDescription.value); };
	}

	std::vector<T> const* operator->() const {
		return &mOptionDescription.value;
	}
	std::vector<T> const& operator*() const {
		return mOptionDescription.value;
	}
};
template<typename T>
class Option<std::set<T>> : public BaseOption {
private:
	OptionDescription<std::set<T>> mOptionDescription;
public:
	Option(Section* _section, std::string const& _name, std::set<T> const& _default, std::string const& _description)
		: Option(_section, _name, _default, _description, [](std::set<T> const&){}) {
	}

	Option(Section* _section, std::string _varName, std::set<T> const& _default, std::string const& _description, std::function<void(std::set<T> const&)> _func)
		: BaseOption(_section, _varName, commonOptions::getParaType<std::set<T>>())
	{
		auto _name = getSectionName() + _varName;
		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		mOptionDescription.optionName   = _name;
		mOptionDescription.description  = _description;
		mOptionDescription.defaultValue = _default;
		mOptionDescription.value        = _default;

		mParseFunction = [&](std::string const& _name) {
			std::stringstream ss;
			ss<<_name;
			T t;
			ss>>t;
			mOptionDescription.value.push_back(t);
		};
		AllOptions::printMap()[_name] = [=]() {
			std::stringstream ss;
			ss<<"--"<<mOptionDescription.optionName<<" { ";
			for (auto const& v : mOptionDescription.defaultValue) {
				ss << v << ", ";
			}
			ss<<"}";
			while(ss.str().length() < 32) {
				ss<<" ";
			}
			ss<<mOptionDescription.description;
			std::cout<<ss.str()<<std::endl;
		};
		AllOptions::printShellComplMap()[_name] = [=]() {
			std::cout<<"--"<<mOptionDescription.optionName<<" ";
		};
		mPreParseFunction  = [&]() { mOptionDescription.value.clear(); };
		mPostParseFunction = [this, _func]() { _func(mOptionDescription.value); };
	}

	std::set<T> const* operator->() const {
		return &mOptionDescription.value;
	}
	std::set<T> const& operator*() const {
		return mOptionDescription.value;
	}
};

}
