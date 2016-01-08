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

		mOptionDescription = std::make_shared<OptionDescription<T>>();
		mOptionDescription->optionName   = _name;
		mOptionDescription->description  = _description;
		mOptionDescription->defaultValue = _default;
		mOptionDescription->value        = _default;

	}
	bool simpleParse(std::vector<std::string> const& _params) override {
		if (_params.size() != 1) {
			//!TODO better message needed
			std::cerr << "Option only takes one argument" << std::endl;
			return false;
		}

		std::stringstream ss;
		ss << _params[0];
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
				std::cerr<<"Wrong option: "<<getName()<<" doesn't accept: "<<_params[0]<<std::endl;
				return false;
			}
		}
		return true;
	};


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

}
