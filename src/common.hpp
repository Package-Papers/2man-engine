#pragma once
#ifndef TME_COMMON
#define TME_COMMON

#define cast(T, v) (static_cast<T>(v))

// Bits
#define MASK(n)     (~((~(( uint32_t ) 0)) << n))
#define MASK1(n, p) ((MASK(n)) << (p))
#define MASK0(n, p) (~(MASK1(n, p)))

[[nodiscard]] constexpr bool int_to_bool(int n) noexcept
{
    return n > 0;
}

#endif /* TME_COMMON */
