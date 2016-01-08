#pragma once

#include <string>
#include <map>
#include <memory>

namespace commonOptions {

/**
 * Description of an option
 */
struct OptionDescription {
	std::string optionName;
	std::string description;
	std::string defaultValue;
};


}
