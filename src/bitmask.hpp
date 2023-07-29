#pragma once
#ifndef TME_BITMASK
#define TME_BITMASK

#include <vector>

namespace tme
{
class Bitmask
{
  public:
    Bitmask();
    void              clear();
    bool              get_bit(int pos) const;
    void              set_bit(int pos, bool on);
    std::vector<bool> get_mask() const;
    void              set_mask(const Bitmask& other);
    void              clear_bit(int pos);

  private:
    std::vector<bool> m_bits;
};
} // namespace tme

#endif /* TME_BITMASK */
