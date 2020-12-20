#pragma once

#include "mstch/mstch.hpp"

namespace mstch {

class is_node_empty {
 public:
  template<class T>
  bool operator()(const T&) const {
    return false;
  }

  bool operator()(std::nullptr_t) const {
    return true;
  }

  bool operator()(long long value) const {
    return value == 0;
  }

  bool operator()(double value) const {
    return value == 0;
  }

  bool operator()(bool value) const {
    return !value;
  }

  bool operator()(const std::string& value) const {
    return value == "";
  }

  bool operator()(const array& array) const {
    return array.size() == 0;
  }
};

}
