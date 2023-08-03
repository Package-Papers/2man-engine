/**
 * This file will determine the level of DEBUG.
 * Note: This value should be set by CMake.
 */

#ifndef TME_DEBUG
#define TME_DEBUG

#ifndef NDEBUG
#define DEBUG

#ifndef DOCTEST_RUN
#define DOCTEST_CONFIG_DISABLE
#endif

#endif

#include "doctest.h"
#include <cassert>

#endif /* TME_DEBUG */
