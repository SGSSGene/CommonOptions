#pragma once


#include <map>
#include <memory>
#include <string>


namespace commonOptions {

/**
 * Description of an option
 */
struct OptionDescription {
	std::string optionName;
	std::string description;
	int         defaultValueLevel;
	std::string defaultValue;
	bool        defaultValueActive;
	std::string value;

	std::function<void()> onDefaultValueChange;
	std::function<void()> onValueChange;

	void changeDefaultValue(std::string const& _defaultValue, int _level) {
		if (_level < defaultValueLevel) return;
		defaultValueLevel = _level;
		defaultValue = _defaultValue;
		if (onDefaultValueChange) {
			onDefaultValueChange();
		}
	}
	void changeValue(std::string const& _value) {
		value = _value;
		if (onValueChange) {
			onValueChange();
		}
	}
};

}
