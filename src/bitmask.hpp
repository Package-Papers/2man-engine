#pragma once
#ifndef TME_BITMASK
#define TME_BITMASK

#include <vector>

namespace tme
{
template <std::size_t N>
class Bitmask
{
  public:
    constexpr Bitmask();
    void              clear();
    bool              get_bit(int pos) const;
    void              set_bit(int pos, bool on);
    std::vector<bool> get_mask() const;
    void              set_mask(const Bitmask& other);
    void              clear_bit(int pos);

    // Checks if the mask contains all bits of the RHS.
    bool contains(const Bitmask& other)
    {
        for (int i = 0; i < N; i++)
        {
            if (other.m_bits[i] && !m_bits[i])
            {
                // mismatch
                return false;
            }
        }
        return true;
    }

  private:
    std::vector<bool> m_bits;
};
} // namespace tme

template <std::size_t N>
constexpr tme::Bitmask<N>::Bitmask()
    : m_bits(N, false){};

template <std::size_t N>
void tme::Bitmask<N>::clear()
{
    m_bits = std::vector<bool>(64, false);
}

template <std::size_t N>
bool tme::Bitmask<N>::get_bit(int pos) const
{
    return m_bits[pos];
}

template <std::size_t N>
void tme::Bitmask<N>::set_bit(int pos, bool on)
{
    m_bits[pos] = on;
}

template <std::size_t N>
std::vector<bool> tme::Bitmask<N>::get_mask() const
{
    return m_bits;
}

template <std::size_t N>
void tme::Bitmask<N>::set_mask(const Bitmask& other)
{
    m_bits = other.m_bits;
}

template <std::size_t N>
void tme::Bitmask<N>::clear_bit(int pos)
{
    m_bits[pos] = false;
}

#endif /* TME_BITMASK */
