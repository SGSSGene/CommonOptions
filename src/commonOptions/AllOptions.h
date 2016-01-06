#pragma once

#include "OptionDescription.h"
#include "ParaType.h"

namespace commonOptions {

class AllOptions {
public:
	template<typename T>
	static OptionDescriptionMap<T>& getOptionDescriptionMap() {
		static OptionDescriptionMap<T> map;
		return map;
	}
	static std::map<std::string, std::function<bool(std::string const&)>>& parseMap() {
		static std::map<std::string, std::function<bool(std::string const&)>> map;
		return map;
	}
	static std::map<std::string, std::function<void()>>& preParseMap() {
		static std::map<std::string, std::function<void()>> map;
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
	static std::map<std::string, std::function<void()>>& printShellComplMap() {
		static std::map<std::string, std::function<void()>> map;
		return map;
	}

	static std::map<std::string, ParaType>& parseParaMap() {
		static std::map<std::string, ParaType> map;
		return map;
	}


};


}
