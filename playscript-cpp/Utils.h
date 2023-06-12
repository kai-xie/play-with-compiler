#pragma once

#include <string>

#include "AntlrPlayScript.h"

namespace play {
namespace utils {

std::string toString(antlrcpp::Any any) {
  if (any.is<std::string>()) {
    return any.as<std::string>();
  } else if (any.is<double>()) {
    return std::to_string(any.as<double>());
  } else if (any.is<float>()) {
    return std::to_string(any.as<float>());
  } else if (any.is<int>()) {
    return std::to_string(any.as<int>());
  } else if (any.is<long>()) {
    return std::to_string(any.as<long>());
  } else if (any.is<short>()) {
    return std::to_string(any.as<short>());
  }
  assert(false && "can not convert antlrcpp::Any to string");
}

double toDouble(antlrcpp::Any any) {
  if (any.is<double>()) {
    return any.as<double>();
  } else if (any.is<float>()) {
    return static_cast<double>(any.as<float>());
  } else if (any.is<long>()) {
    return static_cast<double>(any.as<long>());
  } else if (any.is<int>()) {
    return static_cast<double>(any.as<int>());
  } else if (any.is<short>()) {
    return static_cast<double>(any.as<short>());
  }
  assert(false && "can not conver antlrcpp::Any to double");
}

float toFloat(antlrcpp::Any any) {
  if (any.is<float>()) {
    return any.as<float>();
  } else if (any.is<long>()) {
    return static_cast<float>(any.as<long>());
  } else if (any.is<int>()) {
    return static_cast<float>(any.as<int>());
  } else if (any.is<short>()) {
    return static_cast<float>(any.as<short>());
  }
  assert(false && "can not conver antlrcpp::Any to float");
}

long toLong(antlrcpp::Any any) {
  if (any.is<long>()) {
    return any.as<long>();
  } else if (any.is<int>()) {
    return static_cast<long>(any.as<int>());
  } else if (any.is<short>()) {
    return static_cast<long>(any.as<short>());
  }
  assert(false && "can not conver antlrcpp::Any to long");
}

int toInteger(antlrcpp::Any any) {
  if (any.is<int>()) {
    return any.as<int>();
  } else if (any.is<short>()) {
    return static_cast<int>(any.as<short>());
  }
  assert(false && "can not conver antlrcpp::Any to integer");
}

short toShort(antlrcpp::Any any) {
  if (any.is<short>()) {
    return any.as<short>();
  }
  assert(false && "can not conver antlrcpp::Any to short");
}

}  // namespace utils
}  // namespace play
