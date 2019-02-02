#pragma once

#include <string>
#include <vector>
#include <variant>

namespace mstch {

using citer = std::string::const_iterator;
using criter = std::string::const_reverse_iterator;

citer first_not_ws(citer begin, citer end);
citer first_not_ws(criter begin, criter end);
std::string html_escape(const std::string& str);
criter reverse(citer it);

template<class Visitor, class... Visited>
decltype(auto) visit(Visitor&& visitor, Visited&&... nodes)
{
  return std::visit(std::forward<Visitor>(visitor), nodes.base()...);
}

template <typename T, typename U>
constexpr bool is_v = std::is_same_v<T, U>;

}
