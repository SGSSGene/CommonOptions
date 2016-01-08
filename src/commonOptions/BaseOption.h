#pragma once

#include "ParaType.h"

#include <functional>
#include <string>

namespace commonOptions {

class Section;

class BaseOption {
protected:
	Section*    mSection;
	std::string mName;
	ParaType    mParaType;
public:
	BaseOption(Section* _section, std::string const& _name, ParaType _paraType);
	virtual ~BaseOption();

	auto getSectionName() const -> std::string;
	auto getName() const -> std::string const&;
	auto getParaType() const -> ParaType;

	virtual bool simpleParse(std::string const& _param) = 0;
	virtual void print() const = 0;
	virtual void printShellCompl() const = 0;

};

}
