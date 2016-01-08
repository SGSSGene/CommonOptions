#pragma once


#include <map>
#include <memory>
#include <serializer/serializer.h>
#include <string>


namespace commonOptions {

/**
 * Description of an option
 */
struct OptionDescription {
	std::string optionName;
	std::string description;
	std::string defaultValue;
	std::function<void()> onDefaultValueChange;

	void changeDefaultValue(std::string const& _defaultValue) {
		defaultValue = _defaultValue;
		if (onDefaultValueChange) {
			onDefaultValueChange();
		}
	}

	template<typename Node>
	void serialize(Node& node) {
		node["optionName"]   % optionName;
		node["description"]  % description;
	}
};

}
