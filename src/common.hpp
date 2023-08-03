#pragma once
#ifndef TME_COMMON
#define TME_COMMON

////////////////////////////////////
// Note(Appy): Common utilities.

#define cast(T, v) (static_cast<T>(v))

// Bitwise utilities.
#define MASK(n)     (~((~(( uint32_t ) 0)) << n))
#define MASK1(n, p) ((MASK(n)) << (p))
#define MASK0(n, p) (~(MASK1(n, p)))

[[nodiscard]] constexpr bool int_to_bool(int n) noexcept
{
    return n > 0;
}

#ifndef NDEBUG
#define LOG(x) std::cout << x
#else
#define LOG(x)                                                                                     \
    do                                                                                             \
    {                                                                                              \
    } while (0)
#endif

#endif /* TME_COMMON */
