#include "entity_manager.hpp"
#include "engine/common/debug.hpp"
#include "engine/ecs/core/component.hpp"
#include "entity.hpp"

void EntityManager::execute_actions()
{
    while (!m_action_stack.empty())
    {
        auto action = m_action_stack.back();
        m_action_stack.pop_back();
        action.execute(this);
    }
}

Action* EntityManager::create_action()
{
    m_action_stack.emplace_back();
    return &m_action_stack.back();
}

EntityManager::EntityPool& EntityManager::get_entities()
{
    return m_entity_pool;
}

void EntityManager::destroy_entity(EntityID entity_id)
{
    // Invalidate the index and increment the version.
    auto reuse_index                = get_entity_index(entity_id);
    auto new_version                = get_entity_version(entity_id) + 1;
    auto new_id                     = create_entity_id(INVALID_ENTITY_INDEX, new_version);
    m_entity_pool[reuse_index].m_id = new_id;

    // Reset the components bitmask for reusage.
    m_entity_pool[reuse_index].m_mask.clear();

    // Queue entry back for reuse.
    m_free_entities.push_back(reuse_index);
}

ComponentPool& EntityManager::get_component_pool(std::size_t component_id)
{
#ifndef NDEBUG
    if (component_id >= m_component_pools.size())
    {
        throw std::runtime_error("EntityManager::get_component, out of range");
    }
#endif
    return *m_component_pools[component_id];
}

void* EntityManager::get_entity_component_mem(EntityIndex entity_index, std::size_t component_id)
{
    // Entity has the component, return the memory address.
    ComponentPool& component_pool = get_component_pool(component_id);
    return component_pool.get(entity_index);
}

bool EntityManager::entity_is_valid(EntityID entity_id)
{
    // Return true if version found matches the current one.
    return m_entity_pool[get_entity_index(entity_id)].m_id == entity_id;
}

/*-----------------------*
 |         TESTS         |
 *-----------------------*/

TEST_CASE("Creating new entities")
{
    EntityManager em{};

    auto entity_one   = em.create_entity();
    auto entity_two   = em.create_entity();
    auto entity_three = em.create_entity();

    SUBCASE("Proper increment when creating entities")
    {
        CHECK(entity_one != entity_two);

        // Check that both entities are valid.
        CHECK(is_entity_valid(entity_one));
        CHECK(is_entity_valid(entity_two));
    }

    const auto& entities = em.get_entities();

    SUBCASE("Indexing and retrieving back valid ids")
    {
        CHECK(entity_one == entities[get_entity_index(entity_one)].m_id);
        CHECK(entity_two == entities[get_entity_index(entity_two)].m_id);
    }

    SUBCASE("Checking that newly created entities all have empty masks")
    {
        for (std::size_t i = 0; i < MAX_COMPONENTS; i++)
        {
            CHECK(!entities[get_entity_index(entity_one)].m_mask.get_bit(i));
            CHECK(!entities[get_entity_index(entity_two)].m_mask.get_bit(i));
        }
    }

    struct Age
    {
        int m_i;
    };
    struct Name
    {
        std::string m_name;
    };

    Age*  age  = em.attach<Age>(entity_one);
    Name* name = em.attach<Name>(entity_one);

    auto age_idx  = get_component_id<Age>();
    auto name_idx = get_component_id<Name>();

    SUBCASE("Assigning components")
    {

        // The bitmask should've been set.
        CHECK(entities[get_entity_index(entity_one)].m_mask.get_bit(age_idx));
        CHECK(entities[get_entity_index(entity_one)].m_mask.get_bit(name_idx));

        // Should've stayed the same.
        CHECK(!entities[get_entity_index(entity_three)].m_mask.get_bit(age_idx));
        CHECK(!entities[get_entity_index(entity_three)].m_mask.get_bit(name_idx));

        CHECK(age->m_i == 0);
        CHECK(name->m_name == "");
    }

    // Setting age and name value.
    age->m_i     = 5;
    name->m_name = "Foo";

    SUBCASE("Retrieving component data")
    {
        auto age_v  = em.get<Age>(entity_one);
        auto name_v = em.get<Name>(entity_one);
        CHECK(age_v != nullptr);
        CHECK(name_v != nullptr);
        CHECK(age->m_i == 5);
        CHECK(name->m_name == "Foo");
    }

    SUBCASE("Getting a component data which doesn't exist for an entity")
    {
        auto age_v  = em.get<Age>(entity_three);
        auto name_v = em.get<Name>(entity_three);
        CHECK(age_v == nullptr);
        CHECK(name_v == nullptr);
    }

    // Let's try removing Age component
    em.detach<Age>(entity_one);

    SUBCASE("Component data removal and retrieval")
    {
        auto age_v  = em.get<Age>(entity_one);
        auto name_v = em.get<Name>(entity_one);
        CHECK(name_v != nullptr);

        // This is now gone.
        CHECK(age_v == nullptr);
    }

    // Deletion of entity
    em.attach<Age>(entity_two);
    em.attach<Name>(entity_two);

    SUBCASE("Delete Entity: Entity Validation")
    {
        CHECK(em.get<Age>(entity_two) != nullptr);
        CHECK(em.get<Name>(entity_two) != nullptr);
    }

    auto index_to_reuse    = get_entity_index(entity_two);
    auto ent_2_old_version = get_entity_version(entity_two);
    em.destroy_entity(entity_two);

    SUBCASE("Delete Entity: Checking deleted")
    {
        CHECK(em.get<Age>(entity_two) == nullptr);
        CHECK(em.get<Name>(entity_two) == nullptr);
    }

    auto entity_four = em.create_entity();

    SUBCASE("Delete Entity: Reusing index")
    {
        CHECK(get_entity_index(entity_four) == index_to_reuse);
        CHECK(get_entity_version(entity_four) == ent_2_old_version + 1);

        for (std::size_t i = 0; i < MAX_COMPONENTS; i++)
        {
            CHECK(!entities[get_entity_index(entity_four)].m_mask.get_bit(i));
        }
    }

    SUBCASE("Delete Entity: Normal creation when no reuse")
    {
        auto entity_five = em.create_entity();
        auto new_index   = get_entity_index(entity_five);
        CHECK(get_entity_index(entities[3].m_id) == new_index);
        CHECK(get_entity_version(entities[3].m_id) == 0);
    }
}

TEST_CASE("Proper reusage of index")
{
    EntityManager em{};
    auto&         entities = em.get_entities();

    for (std::size_t i = 0; i < 500; i++)
    {
        em.create_entity();
    }

    for (std::size_t i = 0; i < 123; i++)
    {
        em.destroy_entity(entities[i].m_id);
    }

    for (std::size_t i = 0; i < 333; i++)
    {
        em.create_entity();
    }

    CHECK(entities.size() == 710);

    for (std::size_t i = 0; i < 710; i++)
    {
        em.destroy_entity(entities[i].m_id);
    }

    for (std::size_t i = 0; i < 710; i++)
    {
        CHECK(!is_entity_valid(entities[i].m_id));
    }
}

TEST_CASE("Invalid assignment")
{
    EntityManager em{};
    auto&         entities = em.get_entities();
    auto          e1       = em.create_entity();
    auto          index    = get_entity_index(e1);
    em.destroy_entity(e1);

    // Now we should not be able to assign anything to it.
    class Foo
    {
    };
    class Bar
    {
    };

    em.attach<Foo>(e1);
    em.attach<Bar>(e1);

    CHECK(em.get<Foo>(e1) == nullptr);
    CHECK(em.get<Bar>(e1) == nullptr);
    CHECK(!entities[index].m_mask.get_bit(get_component_id<Foo>()));
    CHECK(!entities[index].m_mask.get_bit(get_component_id<Bar>()));
}

TEST_CASE("Testing functions with multiple components")
{
    EntityManager em{};
    auto&         entities = em.get_entities();
    auto          e1       = em.create_entity();
    auto          index    = get_entity_index(e1);

    // Now we should not be able to assign anything to it.
    class Foo
    {
    };
    class Bar
    {
    };

    auto [foo, bar]   = em.attach<Foo, Bar>(e1);
    auto [gfoo, gbar] = em.get<Foo, Bar>(e1);

    CHECK(em.get<Foo>(e1) == foo);
    CHECK(em.get<Bar>(e1) == bar);
    CHECK(gbar == bar);
    CHECK(gfoo == foo);
    CHECK(entities[index].m_mask.get_bit(get_component_id<Foo>()));
    CHECK(entities[index].m_mask.get_bit(get_component_id<Bar>()));

    SUBCASE("Removing multiple components")
    {
        EntityManager em{};
        auto          e = em.create_entity();

        class Foo
        {
        };
        class Bar
        {
        };
        class Car
        {
        };

        auto [f, b, g] = em.attach<Foo, Bar, Car>(e);

        CHECK(f != nullptr);
        CHECK(b != nullptr);
        CHECK(g != nullptr);

        em.detach<Foo, Bar>(e);

        auto [c, d, z] = em.get<Foo, Bar, Car>(e);

        CHECK(c == nullptr);
        CHECK(d == nullptr);
        CHECK(z != nullptr);
    }
}
