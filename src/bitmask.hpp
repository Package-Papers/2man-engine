#pragma once
#ifndef TME_BITMASK
#define TME_BITMASK

#include <cstdint>

class Bitmask
{
  public:
    void     clear();
    bool     get_bit(int pos) const;
    void     set_bit(int pos, bool on);
    uint32_t get_mask() const;
    void     set_mask(const Bitmask& other);
    void     clear_bit(int pos);

  private:
    uint32_t m_bits;
};

#endif /* TME_BITMASK */
