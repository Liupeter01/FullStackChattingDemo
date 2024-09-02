#pragma once
#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_
#include <memory>

namespace tools {
template <typename _Ty> class ResourcesWrapper {
public:
  ResourcesWrapper(_Ty *ctx) : m_ctx(ctx, [](_Ty *ptr) { /*do nothing*/ }) {}

  _Ty *get() const { return m_ctx.get(); }
  _Ty *operator->() const { return get(); }

private:
  std::shared_ptr<_Ty> m_ctx;
};
} // namespace tools

#endif // !_TOOLS_HPP_
