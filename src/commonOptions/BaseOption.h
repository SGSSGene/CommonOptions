#pragma once

#include <string>

namespace commonOptions {

class Section;

class BaseOption {
private:
	Section*    mSection;
	std::string mName;
public:
	BaseOption(Section* _section, std::string const& _name);
	virtual ~BaseOption();

	auto getSectionName() const -> std::string;
	auto getName() const -> std::string const&;

};

}
