#pragma once
#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_
#include <charconv>
#include <memory>
#include <optional>
#include <string_view>

namespace tools {
template <typename _Ty> class ResourcesWrapper {
public:
  ResourcesWrapper(_Ty *ctx) : m_ctx(ctx, [](_Ty *ptr) { /*do nothing*/ }) {}

  _Ty *get() const { return m_ctx.get(); }
  _Ty *operator->() const { return get(); }

private:
  std::shared_ptr<_Ty> m_ctx;
};

template <typename _Ty>
std::optional<_Ty> string_to_value(std::string_view value) {
  _Ty _temp_res{};
  std::from_chars_result res =
      std::from_chars(value.data(), value.data() + value.size(), _temp_res);
  if (res.ec == std::errc() && res.ptr == value.data() + value.size())
    return _temp_res;
  return std::nullopt;
}

} // namespace tools

#endif // !_TOOLS_HPP_
