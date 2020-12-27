#include <iostream>

#include "mstch/mstch.hpp"
#include "render_context.hpp"

namespace mstch {
	class map_cache : public cache_base {
	public:
		map_cache(std::map<std::string, template_type>&& partials)
		    : m_partials{std::move(partials)} {};

		bool is_valid(const std::string&) const override;
		const template_type& at(const std::string& partial) override;

	private:
		std::map<std::string, template_type> m_partials;
	};
}

using namespace mstch;

std::function<std::string(const std::string&)> mstch::config::escape;

mstch::cache_base::cache_base() = default;
mstch::cache_base::~cache_base() = default;

mstch::cache_base::cache_base(cache_base&&) = default;
mstch::cache_base::cache_base(const cache_base&) = default;
mstch::cache_base& mstch::cache_base::operator=(cache_base&&) = default;
mstch::cache_base& mstch::cache_base::operator=(const cache_base&) = default;

bool mstch::cache::is_valid(const std::string& /*partial_name*/) const {
	return true;
}

const mstch::template_type& mstch::cache::at(const std::string& partial_name) {
  auto it = m_loaded.lower_bound(partial_name);
  if (it == m_loaded.end() || it->first != partial_name)
    it = m_loaded.insert(it, { partial_name,{ load(partial_name) } });
  else if (need_update(partial_name))
    it->second = load(partial_name);
  return it->second;
}

std::string mstch::cache::render(
    const std::string& partial, const node& root)
{
  return render_context(root, *this).render_partial(partial, {});
}

bool mstch::map_cache::is_valid(const std::string& partial_name) const {
	return m_partials.count(partial_name) > 0;
}

const mstch::template_type& mstch::map_cache::at(const std::string& partial_name) {
	auto it = m_partials.find(partial_name);
	if (it == m_partials.end()) {
		static template_type empty{};
	}
	return it->second;
}

std::string mstch::render(
    const std::string& tmplt,
    const node& root,
    const std::map<std::string,std::string>& partials)
{
  std::map<std::string, template_type> partial_templates;
  for (auto& partial: partials)
    partial_templates.insert({partial.first, {partial.second}});

  map_cache cache{std::move(partial_templates)};
  return render_context(root, cache).render(tmplt);
}
