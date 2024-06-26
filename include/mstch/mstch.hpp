#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <functional>

#include <variant>

namespace mstch {

struct config {
  static std::function<std::string(const std::string&)> escape;
};

namespace internal {

template<class N>
class object_t {
 public:
  const N& at(const std::string& name) const {
    cache[name] = (methods.at(name))();
    return cache[name];
  }

  bool has(const std::string name) const {
    return methods.find(name) != methods.end();
  }

 protected:
  template<class S>
  void register_methods(S* s, std::map<std::string,N(S::*)()> methods) {
    for(auto& item: methods)
      this->methods.insert({item.first, std::bind(item.second, s)});
  }

 private:
  std::map<std::string, std::function<N()>> methods;
  mutable std::map<std::string, N> cache;
};

template<class T, class N>
class is_fun {
 private:
  using not_fun = char;
  using fun_without_args = char[2];
  using fun_with_args = char[3];
  template <typename U, U> struct really_has;
  template <typename C> static fun_without_args& test(
      really_has<N(C::*)() const, &C::operator()>*);
  template <typename C> static fun_with_args& test(
      really_has<N(C::*)(const std::string&) const,
      &C::operator()>*);
  template <typename> static not_fun& test(...);

 public:
  static bool const no_args = sizeof(test<T>(0)) == sizeof(fun_without_args);
  static bool const has_args = sizeof(test<T>(0)) == sizeof(fun_with_args);
};

template<class N>
using node_renderer = std::function<std::string(const N& n)>;

template<class N>
class lambda_t {
 public:
  template<class F>
  lambda_t(F f, typename std::enable_if<is_fun<F, N>::no_args>::type* = 0):
      fun([f](node_renderer<N> renderer, const std::string&) {
        return renderer(f());
      })
  {
  }

  template<class F>
  lambda_t(F f, typename std::enable_if<is_fun<F, N>::has_args>::type* = 0):
      fun([f](node_renderer<N> renderer, const std::string& text) {
        return renderer(f(text));
      })
  {
  }

  std::string operator()(node_renderer<N> renderer,
      const std::string& text = "") const
  {
    return fun(renderer, text);
  }

 private:
  std::function<std::string(node_renderer<N> renderer, const std::string&)> fun;
};

template<class N>
struct callback_t {
	virtual ~callback_t() = default;
	virtual const N& at(const std::string& name) const = 0;
	virtual bool has(const std::string& name) const = 0;
	virtual std::vector<std::string> debug_all_keys() const = 0;
};

}

struct node : std::variant<
    std::monostate, std::nullptr_t, std::string, long long, double, bool,
    internal::lambda_t<node>,
    std::shared_ptr<internal::object_t<node>>,
    std::shared_ptr<internal::callback_t<node>>,
    std::map<std::string, node>,
    std::vector<node>
> {
  using lambda_type = internal::lambda_t<node>;
  using object_type = internal::object_t<node>;
  using callback_type = internal::callback_t<node>;
  using map_type = std::map<std::string, node>;
  using vector_type = std::vector<node>;

  using base_type = std::variant<
    std::monostate, std::nullptr_t, std::string, long long, double, bool,
    lambda_type, std::shared_ptr<object_type>, std::shared_ptr<callback_type>, map_type, vector_type
  >;

  using base_type::base_type;

  node() : base_type(std::in_place_type<std::monostate>) {}

  node(const char* value) : base_type(std::in_place_type<std::string>, value) {}

  base_type& base() { return *this; }
  base_type const& base() const { return *this; }
};

using object = internal::object_t<node>;
using callback = internal::callback_t<node>;
using lambda = internal::lambda_t<node>;
using map = std::map<std::string, node>;
using array = std::vector<node>;

std::string render(
    const std::string& tmplt,
    const node& root,
    const std::map<std::string,std::string>& partials =
        std::map<std::string,std::string>());

class template_type;

class cache_base {
public:
	cache_base();
	virtual ~cache_base();

	cache_base(cache_base&&);
	cache_base(const cache_base&);
	cache_base& operator=(cache_base&&);
	cache_base& operator=(const cache_base&);

	virtual const template_type& at(const std::string&) = 0;
	virtual bool is_valid(const std::string&) const = 0;
};

class cache : public cache_base {
public:
  cache();
  ~cache();

  cache(cache&&);
  cache(const cache&);
  cache &operator=(cache&&);
  cache &operator=(const cache&);

  bool is_valid(const std::string &) const override;
  const template_type &at(const std::string &) override;
  std::string render(const std::string &partial, const node &root);

protected:
	virtual std::string load(const std::string& partial) = 0;
	virtual bool need_update(const std::string& partial) const = 0;

private:
	std::map<std::string, template_type> m_loaded;
};

}
