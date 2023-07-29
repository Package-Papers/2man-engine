#include "bitmask.hpp"

void Bitmask::clear()
{
    m_bits = 0;
}

bool Bitmask::get_bit(int pos) const
{
    return (m_bits >> pos) == 1;
}

void Bitmask::set_bit(int pos, bool on)
{
    if (on)
    {
        m_bits |= (1 << pos);
    }
    else
    {
        clear_bit(pos);
    }
}

uint32_t Bitmask::get_mask() const
{
    return m_bits;
}

void Bitmask::set_mask(const Bitmask& other)
{
    m_bits = other.m_bits;
}

void Bitmask::clear_bit(int pos)
{
    m_bits &= (~(1 << pos));
}
