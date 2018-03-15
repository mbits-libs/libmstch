#include <iostream>

#include "mstch/mstch.hpp"
#include "render_context.hpp"

using namespace mstch;

std::function<std::string(const std::string&)> mstch::config::escape;

mstch::cache::cache() = default;
mstch::cache::~cache() = default;

mstch::cache::cache(cache&&) = default;
mstch::cache::cache(const cache&) = default;
mstch::cache& mstch::cache::operator=(cache&&) = default;
mstch::cache& mstch::cache::operator=(const cache&) = default;

const mstch::template_type& mstch::cache::at(const std::string& partial_name) {
  auto it = m_loaded.lower_bound(partial_name);
  if (it == m_loaded.end() || it->first != partial_name)
    it = m_loaded.insert(it, { partial_name,{ load(partial_name) } });
  return it->second;
}
std::string mstch::cache::render(
    const std::string& partial, const node& root)
{
  return render_context(root, *this).render_partial(partial, {});
}
