#include "BaseOption.h"
#include "Section.h"

#include "AllOptions.h"

namespace commonOptions {

BaseOption::BaseOption(Section* _section, std::string const& _name, ParaType _paraType)
	: mSection (_section)
	, mName (_name)
	, mParaType (_paraType)
{
	mParseFunction     = [](std::string const&) { return true; };
	mPreParseFunction  = [] {};
	mPostParseFunction = [] {};
	AllOptions::baseOptionMap()[getSectionName() + mName] = this;
}

BaseOption::~BaseOption() {
	AllOptions::baseOptionMap().erase(getSectionName() + mName);

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
auto BaseOption::getParseFunction() const -> std::function<bool(std::string const&)> {
	return mParseFunction;
}
auto BaseOption::getPreParseFunction() const -> std::function<void()> {
	return mPreParseFunction;
}
auto BaseOption::getPostParseFunction() const -> std::function<void()> {
	return mPostParseFunction;
}

}
