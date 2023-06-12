#pragma once

#include <string>

#include "AntlrPlayScript.h"

namespace play {
namespace utils {

std::string toString(antlrcpp::Any any);

double toDouble(antlrcpp::Any any);

float toFloat(antlrcpp::Any any);

long toLong(antlrcpp::Any any);

int toInteger(antlrcpp::Any any);

short toShort(antlrcpp::Any any);

}  // namespace utils
}  // namespace play
