#pragma once

#include "Base.h"
#include "Types/CLValue.h"
#include "nlohmann/json.hpp"

namespace Casper {
struct NamedArg {
  std::string name;
  CLValue value;

  NamedArg() {}
};

// to_json of NamedArg
inline void to_json(nlohmann::json& j, const NamedArg& p) {
  j = nlohmann::json::array({p.name, p.value});
}

// from_json of NamedArg
inline void from_json(const nlohmann::json& j, NamedArg& p) {
  if (j.at(0).is_string()) {
    j.at(0).get_to(p.name);
    j.at(1).get_to(p.value);
  } else if (j.at(1).is_string()) {
    j.at(0).get_to(p.value);
    j.at(1).get_to(p.name);
  } else {
    throw std::runtime_error("args must be a string or a CLValue (NamedArg)");
  }
}

}  // namespace Casper
