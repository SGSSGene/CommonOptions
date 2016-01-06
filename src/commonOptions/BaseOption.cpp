#include "BaseOption.h"
#include "Section.h"

namespace commonOptions {

BaseOption::BaseOption(Section* _section, std::string const& _name)
	: mSection (_section)
	, mName ( _name) {
}

BaseOption::~BaseOption() {
}

auto BaseOption::getSectionName() const -> std::string {
	return mSection->fullName();
}

auto BaseOption::getName() const -> std::string const& {
	return mName;
}


}
