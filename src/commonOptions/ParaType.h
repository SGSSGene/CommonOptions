#pragma once

#include <cstdint>
#include <list>
#include <set>
#include <string>
#include <vector>

namespace commonOptions {

enum class ParaType { None, One, Multi, Complex };


template<typename T>
struct ParaTypeInfo {
	static ParaType type() {
		return ParaType::Complex;
	}
};

#define PARATYPEINFOONE(TYPE) \
template<> \
struct ParaTypeInfo<TYPE> { \
	static ParaType type() { \
		return ParaType::One; \
	} \
};

PARATYPEINFOONE(double);
PARATYPEINFOONE(float);
PARATYPEINFOONE(int64_t);
PARATYPEINFOONE(uint64_t);
PARATYPEINFOONE(int32_t);
PARATYPEINFOONE(uint32_t);
PARATYPEINFOONE(int16_t);
PARATYPEINFOONE(uint16_t);
PARATYPEINFOONE(int8_t);
PARATYPEINFOONE(uint8_t);
PARATYPEINFOONE(std::string);

template<typename T>
struct ParaTypeInfo<std::vector<T>> {
	static ParaType type() {
		return ParaType::Multi;
	}
};
template<typename T>
struct ParaTypeInfo<std::set<T>> {
	static ParaType type() {
		return ParaType::Multi;
	}
};


template<typename T>
ParaType getParaType() {
	return ParaTypeInfo<T>::type();
}

}
