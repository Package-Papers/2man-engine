#pragma once
#ifndef NON_COPYABLE
#define NON_COPYABLE

namespace tme
{
class NonCopyable
{
  protected:
    NonCopyable()  = default;
    ~NonCopyable() = default;

  public:
    NonCopyable(const NonCopyable&)            = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
} // namespace tme
#endif /* NON_COPYABLE */
