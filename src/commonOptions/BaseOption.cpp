#include "BaseOption.h"
#include "Section.h"

#include "OptionDescription.h"
#include "ParaType.h"

namespace commonOptions {

BaseOption::BaseOption(Section* _section, std::string const& _name, ParaType _paraType)
	: mSection (_section)
	, mName (_name)
	, mParaType (_paraType)
{
}

BaseOption::~BaseOption() {

}

auto BaseOption::getSectionName() const -> std::string {
	return mSection->fullName();
}

auto BaseOption::getName() const -> std::string const& {
	return mName;
}
auto BaseOption::getParaType() const -> ParaType {
	return mParaType;
}

}
