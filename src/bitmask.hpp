#pragma once
#ifndef TME_BITMASK
#define TME_BITMASK

#include <cstddef>
#include <vector>

#include "debug.hpp"

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

    [[nodiscard]] std::size_t size() const noexcept
    {
        return m_size;
    }

    // Checks if the mask contains all bits of the RHS. MUST HAVE THE SAME SIZE.
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
    const std::size_t m_size;
};
} // namespace tme

template <std::size_t N>
constexpr tme::Bitmask<N>::Bitmask()
    : m_bits(N, false)
    , m_size(N){};

template <std::size_t N>
void tme::Bitmask<N>::clear()
{
    m_bits = std::vector<bool>(N, false);
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

/*
 * Tests.
 */

TEST_CASE("Bitmask initialization")
{
    // Expect all to be false;
    {
        tme::Bitmask<512> empty_mask;

        for (std::size_t i = 0; i < 512; i++)
        {
            CHECK(!empty_mask.get_bit(i));
        }
    }
    {
        tme::Bitmask<1024> empty_mask;

        for (std::size_t i = 0; i < 1024; i++)
        {
            CHECK(!empty_mask.get_bit(i));
        }
    }
}

TEST_CASE("Bitmask clearing")
{
    tme::Bitmask<200> mask;
    for (std::size_t i = 0; i < 200; i++)
    {
        mask.set_bit(i, (i & 1) == 1);
    }

    // Verify the pattern
    for (std::size_t i = 0; i < 200; i++)
    {
        if ((i & 1) == 1)
        {
            CHECK(mask.get_bit(i));
        }
        else
        {
            CHECK(!mask.get_bit(i));
        }
    }

    mask.clear();

    // Should retain the same size.
    CHECK(mask.size() == 200);

    // Should all be empty.
    for (std::size_t i = 0; i < 200; i++)
    {
        CHECK(!mask.get_bit(i));
    }
}

TEST_CASE("Bitmask contains")
{
    tme::Bitmask<10> mask_1;
    mask_1.set_bit(0, true);
    mask_1.set_bit(9, true);

    tme::Bitmask<10> mask_2;
    CHECK(!mask_2.contains(mask_1));

    mask_2.set_bit(0, true);
    mask_2.set_bit(9, true);

    CHECK(mask_2.contains(mask_1));

    tme::Bitmask<10> mask_3;
    for (std::size_t i = 0; i < mask_3.size(); i++)
    {
        mask_3.set_bit(i, true);
    }

    CHECK(mask_3.contains(mask_1));
    mask_3.clear();
    CHECK(!mask_3.contains(mask_1));

    mask_3.set_mask(mask_1);
    CHECK(mask_3.contains(mask_1));
    CHECK(mask_1.contains(mask_3));
}

#endif /* TME_BITMASK */
