#pragma once

#include "OptionDescription.h"
#include "ParaType.h"
#include "BaseOption.h"

#include <serializer/serializer.h>

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
	std::shared_ptr<OptionDescription> mOptionDescription;
	std::shared_ptr<T>                 mCurrentValue;
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

		mOptionDescription = std::make_shared<OptionDescription>();
		mOptionDescription->optionName   = _name;
		mOptionDescription->description  = _description;

		mOptionDescription->defaultValue = serializer::yaml::writeAsString(_default);
		mCurrentValue = std::make_shared<T>(_default);
	}
	bool simpleParse(std::string const& _param) override {
		serializer::yaml::readFromString(_param, *mCurrentValue);

		if (onlyPossibleValues) {
			bool foundValue {false};
			for (auto const& s : possibleValues) {
				if (*mCurrentValue == s) {
					foundValue = true;
					break;
				}
			}
			if (not foundValue) {
				std::cerr<<"Wrong option: "<<getName()<<" doesn't accept: "<<_param<<std::endl;
				return false;
			}
		}
		return true;
	};


	T const* operator->() const {
		return mCurrentValue.get();
	}
	T const& operator*() const {
		return *mCurrentValue;
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
