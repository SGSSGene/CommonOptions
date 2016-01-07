#pragma once

#include "OptionDescription.h"
#include "ParaType.h"
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
	std::shared_ptr<OptionDescription<T>> mOptionDescription;
public:
	Option(Section* _section, std::string const& _name, T const& _default, std::string const& _description)
		: Option(_section, _name, _default, {}, _description) {
	}
	Option(Section* _section, std::string const& _varName, T const& _default, std::set<T> const& _list, std::string const& _description)
		: BaseOption(_section, _varName, commonOptions::getParaType<T>())
	{
		onlyPossibleValues = not _list.empty();
		possibleValues = _list;
		possibleValues.insert(_default);

		auto _name = getSectionName() + _varName;
		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		mOptionDescription = std::shared_ptr<OptionDescription<T>>();
		mOptionDescription->optionName   = _name;
		mOptionDescription->description  = _description;
		mOptionDescription->defaultValue = _default;
		mOptionDescription->value        = _default;

		mParseFunction = [&](std::string const& _value) {
			std::stringstream ss;
			ss<<_value;
			ss>>mOptionDescription->value;
			if (onlyPossibleValues) {
				bool foundValue {false};
				for (auto const& s : possibleValues) {
					if (mOptionDescription->value == s) {
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
	}

	T const* operator->() const {
		return &mOptionDescription->value;
	}
	T const& operator*() const {
		return mOptionDescription->value;
	}
	void print() const override {
		std::stringstream ss;
		ss<<"--"<<mOptionDescription->optionName;
		if (mParaType != ParaType::None) {
			ss<<" "<<mOptionDescription->defaultValue;
		}
		while(ss.str().length() < 32) {
			ss<<" ";
		}
		ss<<mOptionDescription->description;
		std::cout<<ss.str()<<std::endl;
	}
	void printShellCompl() const override {
		std::cout<<"--"<<mOptionDescription->optionName<<" ";
	}


};
template<typename T>
class Option<std::vector<T>> : public BaseOption {
private:
	std::shared_ptr<OptionDescription<std::vector<T>>> mOptionDescription;
public:
	Option(Section* _section, std::string const& _varName, std::vector<T> const& _default, std::string const& _description)
		: BaseOption(_section, _varName, commonOptions::getParaType<std::vector<T>>())
	{
		auto _name = getSectionName() + _varName;

		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		mOptionDescription = std::make_shared<OptionDescription<std::vector<T>>>();
		mOptionDescription->optionName   = _name;
		mOptionDescription->description  = _description;
		mOptionDescription->defaultValue = _default;
		mOptionDescription->value        = _default;

		mParseFunction = [&](std::string const& _value) {
			std::stringstream ss;
			ss<<_value;
			T t;
			ss>>t;
			mOptionDescription->value.push_back(t);
			return true;
		};
	}

	std::vector<T> const* operator->() const {
		return &mOptionDescription->value;
	}
	std::vector<T> const& operator*() const {
		return mOptionDescription->value;
	}
	void print() const override {
		std::stringstream ss;
		ss<<"--"<<mOptionDescription->optionName<<" { ";
		for (auto const& v : mOptionDescription->defaultValue) {
			ss << v << ", ";
		}
		ss<<"}";
		while(ss.str().length() < 32) {
			ss<<" ";
		}
		ss<<mOptionDescription->description;
		std::cout<<ss.str()<<std::endl;
	}
	void printShellCompl() const override {
		std::cout<<"--"<<mOptionDescription->optionName<<" ";
	}
};

template<typename T>
class Option<std::set<T>> : public BaseOption {
private:
	std::shared_ptr<OptionDescription<std::set<T>>> mOptionDescription;
public:

	Option(Section* _section, std::string const& _varName, std::set<T> const& _default, std::string const& _description)
		: BaseOption(_section, _varName, commonOptions::getParaType<std::set<T>>())
	{
		auto _name = getSectionName() + _varName;
		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		mOptionDescription = std::make_shared<OptionDescription<std::set<T>>>();
		mOptionDescription->optionName   = _name;
		mOptionDescription->description  = _description;
		mOptionDescription->defaultValue = _default;
		mOptionDescription->value        = _default;

		mParseFunction = [&](std::string const& _name) {
			std::stringstream ss;
			ss<<_name;
			T t;
			ss>>t;
			mOptionDescription->value.push_back(t);
		};
	}

	std::set<T> const* operator->() const {
		return &mOptionDescription->value;
	}
	std::set<T> const& operator*() const {
		return mOptionDescription->value;
	}
	void print() const override {
		std::stringstream ss;
		ss<<"--"<<mOptionDescription->optionName<<" { ";
		for (auto const& v : mOptionDescription->defaultValue) {
			ss << v << ", ";
		}
		ss<<"}";
		while(ss.str().length() < 32) {
			ss<<" ";
		}
		ss<<mOptionDescription->description;
		std::cout<<ss.str()<<std::endl;
	}
	void printShellCompl() const override {
		std::cout<<"--"<<mOptionDescription->optionName<<" ";
	}
};

}
