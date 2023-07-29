#include "bitmask.hpp"

tme::Bitmask::Bitmask()
    : m_bits(64, false){};

void tme::Bitmask::clear()
{
    m_bits = std::vector<bool>(64, false);
}

bool tme::Bitmask::get_bit(int pos) const
{
    return m_bits[pos];
}

void tme::Bitmask::set_bit(int pos, bool on)
{
    m_bits[pos] = on;
}

std::vector<bool> tme::Bitmask::get_mask() const
{
    return m_bits;
}

void tme::Bitmask::set_mask(const Bitmask& other)
{
    m_bits = other.m_bits;
}

void tme::Bitmask::clear_bit(int pos)
{
    m_bits[pos] = false;
}