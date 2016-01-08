#pragma once

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

template<typename T>
struct CurrentValue {
	T    value;
	bool defaultValue;
};

/**
 * This type must be convertable by using stringstream
 */
template<typename T>
class Option : public BaseOption {
private:
	bool               onlyPossibleValues;
	std::set<T>        possibleValues;
	std::shared_ptr<CurrentValue<T>>   mCurrentValue;
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

		createDescription(serializer::yaml::writeAsString(_default), _description);

		mCurrentValue = std::make_shared<CurrentValue<T>>();

		mCurrentValue->value = _default;
		mCurrentValue->defaultValue = true;

		mOptionDescription->onDefaultValueChange = [&] {
			if (mCurrentValue->defaultValue) {
				serializer::yaml::readFromString(mOptionDescription->defaultValue, mCurrentValue->value);
			}
		};
	}
	bool simpleParse(std::string const& _param) override {
		serializer::yaml::readFromString(_param, mCurrentValue->value);

		if (onlyPossibleValues) {
			bool foundValue {false};
			for (auto const& s : possibleValues) {
				if (mCurrentValue->value == s) {
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
		return &mCurrentValue->value;
	}

	T const& operator*() const {
		return mCurrentValue->value;
	}

	void resetToDefault() {
		mCurrentValue->defaultValue = true;
		serializer::yaml::readFromString(mOptionDescription->defaultValue, mCurrentValue->value);
	}
	void setValue(T const& t) {
		mCurrentValue->defaultValue = false;
		mCurrentValue->value = t;
	}
	void setValue(T&& t) {
		mCurrentValue->defaultValue = false;
		mCurrentValue->value = t;
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
