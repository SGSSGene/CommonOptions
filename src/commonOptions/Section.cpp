#include "Section.h"
#include "commonOptions.h"

namespace commonOptions {

Section::Section()
	: mParent(nullptr)
{
}

Section::Section(Section* _parent, std::string const& _name)
	: mParent (_parent)
	, mName   (_name)
{
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
