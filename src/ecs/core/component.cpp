#include "component.hpp"

uint32_t g_component_count = 0;

TEST_CASE("Registering components")
{
  struct C1;
  struct C2;
  struct C3;
  struct C4;
  struct C5;
  struct C6;

  // New entries.
  auto c1 = get_component_id<C1>(); // 0
  auto c2 = get_component_id<C2>(); // 1
  auto c3 = get_component_id<C3>(); // 2

  CHECK(c1 == 0); 
  CHECK(c2 == 1); 
  CHECK(c3 == 2); 

  // Not new, ID should match.
  auto c4 = get_component_id<C1>();
  auto c5 = get_component_id<C2>();
  auto c6 = get_component_id<C3>();

  CHECK(c1 == c4);
  CHECK(c2 == c5);
  CHECK(c3 == c6);

  // New IDS.
  c5 = get_component_id<C6>(); // 3
  c6 = get_component_id<C5>(); // 4
  c4 = get_component_id<C4>(); // 5
  
  // Should not longer be the same.
  CHECK(c1 != c4);
  CHECK(c2 != c5);
  CHECK(c3 != c6);

  CHECK(c5 == 3); // 3
  CHECK(c6 == 4);
  CHECK(c4 == 5);
}
