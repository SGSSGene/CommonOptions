#ifndef COMMANDLINEPARSER_COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_COMMANDLINEPARSER_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <sstream>

namespace commonOptions {

template<typename T>
struct OptionDescription {
	std::string optionName;
	std::string description;
	T defaultValue;
	T value;
};

template<typename T>
using OptionDescriptionMap = std::map<std::string, std::shared_ptr<OptionDescription<T>>>;

class AllOptions {
public:
	template<typename T>
	static OptionDescriptionMap<T>& getOptionDescriptionMap() {
		static OptionDescriptionMap<T> map;
		return map;
	}
	static std::map<std::string, std::function<void(std::string const&)>>& parseMap() {
		static std::map<std::string, std::function<void(std::string const&)>> map;
		return map;
	}
	static std::map<std::string, std::function<void()>>& postParseMap() {
		static std::map<std::string, std::function<void()>> map;
		return map;
	}
	static std::map<std::string, std::function<void()>>& printMap() {
		static std::map<std::string, std::function<void()>> map;
		return map;
	}

	static std::map<std::string, bool>& parseParaMap() {
		static std::map<std::string, bool> map;
		return map;
	}


};

/**
 * This type must be convertable by using stringstream
 */
template<typename T>
class Option {
private:
	std::shared_ptr<OptionDescription<T>> value;
public:
	Option(std::string const& _name, T const& _default, std::string const& _description)
		: Option(_name, _default, _description, [](T const&){}) {
	}

	Option(std::string const& _name, T const& _default, std::string const& _description, std::function<void(T const&)> _func) {

		auto& map = AllOptions::getOptionDescriptionMap<T>();
		if (map[_name] == nullptr) {
			map[_name] = std::make_shared<OptionDescription<T>>();
			map[_name]->optionName   = _name;
			map[_name]->description  = _description;
			map[_name]->defaultValue = _default;
			map[_name]->value        = _default;
		}
		value = map[_name];
		AllOptions::parseMap()[_name] = [&](std::string const& _name) {
			std::stringstream ss;
			ss<<_name;
			ss>>value->value;
		};
		AllOptions::printMap()[_name] = [=]() {
			std::stringstream ss;
			ss<<"--"<<value->optionName<<" "<<value->defaultValue;
			while(ss.str().length() < 32) {
				ss<<" ";
			}
			ss<<value->description;
			std::cout<<ss.str()<<std::endl;
		};
		AllOptions::parseParaMap()[_name] = true;
		AllOptions::postParseMap()[_name] = [this, _func]() { _func(value->value); };
	}

	T const* operator->() const {
		return &value->value;
	}
	T const& operator*() const {
		return value->value;
	}
};
class Switch : public Option<bool> {
public:
	Switch(std::string const& _name, std::string const& _description)
		: Switch(_name, _description, []() {}) {
	}
	Switch(std::string const& _name, std::string const& _description, std::function<void()> _func)
		: Option(_name, false, _description) {
		AllOptions::parseParaMap()[_name] = false;
		AllOptions::postParseMap()[_name] = _func;
	}


	operator bool() {
		return **this;
	}
};

inline void print() {
	for (auto f : AllOptions::printMap()) {
		f.second();
	}
}

/**
 * This is a very simple command line parser
 * It can parse stuff like
 * --option
 * --option value
 * --opiton=value
 *
 *  it will ignore everything else on the command line. This is needed because boost::
 *  program_options is handling unknown options very badly
 */
inline void parse(int argc, char const* const* argv) {
	std::map<std::string, std::string> options;

	for (int i(0); i<argc; ++i) {
		std::string arg = argv[i];
		if (0 == arg.compare(0, 2, "--")) {
			arg = arg.substr(2); // cut of first two symbols
			size_t equalSignPos = arg.find("=");
			if (equalSignPos != std::string::npos) {
				std::string key   = arg.substr(0, equalSignPos);
				std::string value = arg.substr(equalSignPos+1);
				AllOptions::parseMap()[key](value);
				AllOptions::postParseMap()[key]();
			} else if (i+1 < argc
					   && std::string(argv[i+1]).compare(0, 2, "--") != 0) {
				std::string key   = arg;

				if (AllOptions::parseParaMap().at(key)) {
					std::string value = argv[i+1];
					AllOptions::parseMap()[key](value);
					AllOptions::postParseMap()[key]();
					++i;
				} else {
					AllOptions::parseMap()[key]("1");
				}
			} else {
				AllOptions::parseMap()[arg]("1");
				AllOptions::postParseMap()[arg]();

			}
		}
	}
}

}

#endif
