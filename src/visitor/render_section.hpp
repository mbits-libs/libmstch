#pragma once

#include "render_context.hpp"
#include "mstch/mstch.hpp"
#include "utils.hpp"
#include "render_node.hpp"

namespace mstch {

class render_section {
 public:
  enum class flag { none, keep_array };
  render_section(
      render_context& ctx,
      const template_type& section,
      const delim_type& delims,
      flag p_flag = flag::none):
      m_ctx(ctx), m_section(section), m_delims(delims), m_flag(p_flag)
  {
  }

  template<class T>
  std::string operator()(const T& value) const {
	  if constexpr(is_v<T, lambda>) {
		  std::string section_str;
		  for (auto& token : m_section)
			  section_str += token.raw();
		  template_type interpreted{ value([this](const mstch::node& n) {
			  return visit(render_node(m_ctx), n);
		  }, section_str), m_delims };
		  return render_context::push(m_ctx).render(interpreted);
	  } else if constexpr(is_v<T, array>) {
      std::string out;
      if (m_flag == flag::keep_array)
        return render_context::push(m_ctx, value).render(m_section);
      else
        for (auto& item: value)
          out += visit(render_section(
              m_ctx, m_section, m_delims, flag::keep_array), item);
      return out;
	  }
	  return render_context::push(m_ctx, value).render(m_section);
  }

 private:
  render_context& m_ctx;
  const template_type& m_section;
  const delim_type& m_delims;
  flag m_flag;
};

}
