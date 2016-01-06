#pragma once

#include "AllOptions.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

namespace commonOptions {

/**
 * This type must be convertable by using stringstream
 */
template<typename T>
class Option {
private:
	std::string name;
	std::shared_ptr<OptionDescription<T>> value;
	bool        onlyPossibleValues;
	std::set<T> possibleValues;
public:
	Option(std::string const& _name, T const& _default, std::string const& _description)
		: Option(_name, _default, {}, _description, [](T const&){}) {
	}
	Option(std::string const& _name, T const& _default, std::set<T> const& _list, std::string const& _description)
		: Option(_name, _default, _list, _description, [](T const&){}) {
	}
	Option(std::string _name, T const& _default, std::string const& _description, std::function<void(T const&)> _func)
		: Option(_name, _default, {}, _description, _func) {
	}


	Option(std::string _name, T const& _default, std::set<T> const& _list, std::string const& _description, std::function<void(T const&)> _func)
		: name(_name) {
		onlyPossibleValues = not _list.empty();
		possibleValues = _list;
		possibleValues.insert(_default);

		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		auto& map = AllOptions::getOptionDescriptionMap<T>();
		if (map[_name] == nullptr) {
			map[_name] = std::make_shared<OptionDescription<T>>();
			map[_name]->optionName   = _name;
			map[_name]->description  = _description;
			map[_name]->defaultValue = _default;
			map[_name]->value        = _default;
		}
		value = map[_name];
		AllOptions::parseMap()[_name] = [&](std::string const& _value) {
			std::stringstream ss;
			ss<<_value;
			ss>>value->value;
			if (onlyPossibleValues) {
				bool foundValue {false};
				for (auto const& s : possibleValues) {
					if (value->value == s) {
						foundValue = true;
						break;
					}
				}
				if (not foundValue) {
					std::cerr<<"Wrong option: "<<name<<" doesn't accept: "<<_value<<std::endl;
					return false;
				}
			}
			return true;
		};
		AllOptions::printMap()[_name] = [=]() {
			std::stringstream ss;
			ss<<"--"<<value->optionName;
			if (AllOptions::parseParaMap()[_name] != ParaType::None) {
				ss<<" "<<value->defaultValue;
			}
			while(ss.str().length() < 32) {
				ss<<" ";
			}
			ss<<value->description;
			std::cout<<ss.str()<<std::endl;
		};
		AllOptions::printShellComplMap()[_name] = [=]() {
			std::cout<<"--"<<value->optionName<<" ";
		};

		AllOptions::parseParaMap()[_name] = ParaType::One;
		AllOptions::preParseMap()[_name] = []() {};
		AllOptions::postParseMap()[_name] = [this, _func]() { _func(value->value); };
	}

	T const* operator->() const {
		return &value->value;
	}
	T const& operator*() const {
		return value->value;
	}
};
template<typename T>
class Option<std::vector<T>> {
private:
	std::shared_ptr<OptionDescription<std::vector<T>>> value;
public:
	Option(std::string const& _name, std::vector<T> const& _default, std::string const& _description)
		: Option(_name, _default, _description, [](std::vector<T> const&){}) {
	}

	Option(std::string _name, std::vector<T> const& _default, std::string const& _description, std::function<void(std::vector<T> const&)> _func) {
		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		auto& map = AllOptions::getOptionDescriptionMap<std::vector<T>>();
		if (map[_name] == nullptr) {
			map[_name] = std::make_shared<OptionDescription<std::vector<T>>>();
			map[_name]->optionName   = _name;
			map[_name]->description  = _description;
			map[_name]->defaultValue = _default;
			map[_name]->value        = _default;
		}
		value = map[_name];
		AllOptions::parseMap()[_name] = [&](std::string const& _value) {
			std::stringstream ss;
			ss<<_value;
			T t;
			ss>>t;
			value->value.push_back(t);
			return true;
		};
		AllOptions::printMap()[_name] = [=]() {
			std::stringstream ss;
			ss<<"--"<<value->optionName<<" { ";
			for (auto const& v : value->defaultValue) {
				ss << v << ", ";
			}
			ss<<"}";
			while(ss.str().length() < 32) {
				ss<<" ";
			}
			ss<<value->description;
			std::cout<<ss.str()<<std::endl;
		};
		AllOptions::printShellComplMap()[_name] = [=]() {
			std::cout<<"--"<<value->optionName<<" ";
		};

		AllOptions::parseParaMap()[_name] = ParaType::Multi;
		AllOptions::preParseMap()[_name] = [&]() { value->value.clear(); };
		AllOptions::postParseMap()[_name] = [this, _func]() { _func(value->value); };
	}

	std::vector<T> const* operator->() const {
		return &value->value;
	}
	std::vector<T> const& operator*() const {
		return value->value;
	}
};
template<typename T>
class Option<std::set<T>> {
private:
	std::shared_ptr<OptionDescription<std::set<T>>> value;
public:
	Option(std::string const& _name, std::set<T> const& _default, std::string const& _description)
		: Option(_name, _default, _description, [](std::set<T> const&){}) {
	}

	Option(std::string _name, std::set<T> const& _default, std::string const& _description, std::function<void(std::set<T> const&)> _func) {
		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		auto& map = AllOptions::getOptionDescriptionMap<std::set<T>>();
		if (map[_name] == nullptr) {
			map[_name] = std::make_shared<OptionDescription<std::set<T>>>();
			map[_name]->optionName   = _name;
			map[_name]->description  = _description;
			map[_name]->defaultValue = _default;
			map[_name]->value        = _default;
		}
		value = map[_name];
		AllOptions::parseMap()[_name] = [&](std::string const& _name) {
			std::stringstream ss;
			ss<<_name;
			T t;
			ss>>t;
			value->value.push_back(t);
		};
		AllOptions::printMap()[_name] = [=]() {
			std::stringstream ss;
			ss<<"--"<<value->optionName<<" { ";
			for (auto const& v : value->defaultValue) {
				ss << v << ", ";
			}
			ss<<"}";
			while(ss.str().length() < 32) {
				ss<<" ";
			}
			ss<<value->description;
			std::cout<<ss.str()<<std::endl;
		};
		AllOptions::printShellComplMap()[_name] = [=]() {
			std::cout<<"--"<<value->optionName<<" ";
		};
		AllOptions::parseParaMap()[_name] = ParaType::Multi;
		AllOptions::preParseMap()[_name] = [&]() { value->value.clear(); };
		AllOptions::postParseMap()[_name] = [this, _func]() { _func(value->value); };
	}

	std::set<T> const* operator->() const {
		return &value->value;
	}
	std::set<T> const& operator*() const {
		return value->value;
	}
};

}
