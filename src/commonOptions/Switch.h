#pragma once

#include "Option.h"

namespace commonOptions {

class Switch : public Option<bool> {
public:
	Switch(Section* _section, std::string const& _name, std::string const& _description)
		: Switch(_section, _name, _description, []() {}) {
	}
	Switch(Section* _section, std::string _varName, std::string const& _description, std::function<void()> _func)
		: Option(_section, _varName, false, _description) {
		mParaType = ParaType::None;

		auto _name = getSectionName() + _varName;
		std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);

		mPreParseFunction  = []() {};
		mPostParseFunction = _func;
	}


	operator bool() {
		return **this;
	}
};


}
