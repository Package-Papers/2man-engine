#include "component_pool.hpp"

TEST_CASE("Creating a component pool")
{
	struct Pos { int m_x, m_y, m_z; Pos(int x, int y, int z): m_x(x), m_y(y), m_z(z){} };

	// Create a new component pool for `position` component.
	ComponentPool comp = ComponentPool(sizeof(Pos));

	// Get any arbitrary index, it should be null.

	auto index = 16;

	// Retrieve the memory location.
	// Should be random garbage value. (touchable)
	void* mem = comp.get(index);
	CHECK(mem!=nullptr);

	// Now we can try allocating something at the location.
	new (mem) Pos(1, 2, 3);

	CHECK(mem!=nullptr);

	// Now, the data should be there.
	CHECK(static_cast<Pos*>(comp.get(index))->m_x == 1);
	CHECK(static_cast<Pos*>(comp.get(index))->m_y == 2);
	CHECK(static_cast<Pos*>(comp.get(index))->m_z == 3);
}