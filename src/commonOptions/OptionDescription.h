#pragma once

namespace commonOptions {

/**
 * Description of an option
 */
template<typename T>
struct OptionDescription {
	std::string optionName;
	std::string description;
	T defaultValue;
	T value;
};


/**
 * Option description map of a perticular option
 */
template<typename T>
using OptionDescriptionMap = std::map<std::string, std::shared_ptr<OptionDescription<T>>>;


}

