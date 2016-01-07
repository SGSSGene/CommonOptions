#pragma once

#include "OptionDescription.h"
#include "ParaType.h"

namespace commonOptions {

class BaseOption;

class AllOptions {
public:

	template<typename T>
	static OptionDescriptionMap<T>& getOptionDescriptionMap() {
		static OptionDescriptionMap<T> map;
		return map;
	}

	static std::map<std::string, std::function<void()>>& printMap() {
		static std::map<std::string, std::function<void()>> map;
		return map;
	}

	static std::map<std::string, std::function<void()>>& printShellComplMap() {
		static std::map<std::string, std::function<void()>> map;
		return map;
	}

	static std::map<std::string, BaseOption*>& baseOptionMap() {
		static std::map<std::string, BaseOption*> map;
		return map;
	}


};


}
