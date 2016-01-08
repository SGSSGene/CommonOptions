#include "Section.h"
#include "commonOptions.h"

namespace commonOptions {

Section::Section()
	: mParent(nullptr)
{
}
Section::Section(Section const& _other) {
	*this = _other;
}

Section::Section(Section* _parent, std::string const& _name)
	: mParent (_parent)
	, mName   (_name)
{
}

Section& Section::operator=(Section const& _other) {
	mParent = _other.mParent;
	mName   = _other.mName;
	return *this;
}

std::vector<BaseOption*> Section::getVariables() {
	std::vector<BaseOption*> options;
	getVariablesImpl(&options);
	return options;
}

void Section::getVariablesImpl(std::vector<BaseOption*>* options) {
	for (auto& p : mVariables) {
		options->push_back(p.second.get());
	}
	for (auto& child : mChildren) {
		child.second.getVariablesImpl(options);
	}
}

BaseOption* Section::getVariable(std::string const& _name) {
	auto p = getSectionOfVariable(_name);
	if (p.first != this) {
		return p.first->getVariable(p.second);
	}

	if (mVariables.find(_name) != mVariables.end()) {
		return mVariables.at(_name).get();
	}

	return nullptr;
}
OptionDescription* Section::getDescription(std::string const& _name) {
	auto p = getSectionOfVariable(_name);
	if (p.first != this) {
		return p.first->getDescription(p.second);
	}

	if (mDescriptions.find(_name) == mDescriptions.end()) {
		auto& ptr = mDescriptions[_name];
		ptr.reset(new OptionDescription);
		ptr->optionName = _name;
	}
	return mDescriptions.at(_name).get();
}



Section* Section::accessChild(std::string const& _name) {
	auto iter = mChildren.find(_name);
	if (iter == mChildren.end()) {
		iter = mChildren.insert({_name, Section(this, _name)}).first;
	}
	return &iter->second;
}

std::string Section::fullName() const {
	if (not mParent) return "";
	return mParent->fullName() + mName + ".";
}



}
