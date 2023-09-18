#include "entity_capture.hpp"
#include "core/entity_manager.hpp"

TEST_CASE("Capturing drawable entities")
{
    EntityManager em{};

    auto e1 = em.create_entity();
    auto e2 = em.create_entity();
    auto e3 = em.create_entity();
    auto e4 = em.create_entity();
    auto e5 = em.create_entity();
    auto e6 = em.create_entity();

    // Basically a tag.
    struct Drawable
    {
    };

    em.attach<Drawable>(e1);
    em.attach<Drawable>(e2);
    em.attach<Drawable>(e5);

    for (EntityID e : EntityCapture<Drawable>(em))
    {
        if ((e != e1) && (e != e2) && (e != e5))
        {
            CHECK(false);
        }
    }
}

TEST_CASE("Capturing two tags")
{
    EntityManager em{};

    auto e1 = em.create_entity();
    auto e2 = em.create_entity();
    auto e3 = em.create_entity();
    auto e4 = em.create_entity();
    auto e5 = em.create_entity();
    auto e6 = em.create_entity();

    // Basically a tag.
    struct Drawable
    {
    };
    struct Pos
    {
    };
    struct Health
    {
    };

    em.attach<Drawable>(e1);
    em.attach<Health>(e1);

    em.attach<Drawable>(e2);
    em.attach<Health>(e2);
    em.attach<Pos>(e2);

    em.attach<Drawable>(e5);
    em.attach<Pos>(e5);

    auto count_1 = 0;
    for (EntityID e : EntityCapture<Drawable, Pos>(em))
    {
        if ((e == e2) || (e == e5))
        {
            count_1++;
        }
    }
    CHECK(count_1 == 2);

    auto count_2 = 0;
    for (EntityID e : EntityCapture<Drawable>(em))
    {
        if ((e == e2) || (e == e1) || (e == e5))
        {
            count_2++;
        }
    }
    CHECK(count_2 == 3);

    auto count_3 = 0;
    for (EntityID e : EntityCapture<Health>(em))
    {
        if ((e == e1) || (e == e2))
        {
            count_3++;
        }
    }
    CHECK(count_3 == 2);

    // Grab all
    auto count_4 = 0;
    for (EntityID e : EntityCapture<>(em))
    {
        count_4++;
    }
    CHECK(count_4 == 6);

    for (EntityID e : EntityCapture<Health, Pos, Drawable>(em))
    {
        CHECK(e == e2);
    }
}

TEST_CASE("Removal and capturing")
{
    EntityManager em{};

    auto e1 = em.create_entity();
    auto e2 = em.create_entity();
    auto e3 = em.create_entity();
    auto e4 = em.create_entity();
    auto e5 = em.create_entity();
    auto e6 = em.create_entity();

    // Basically a tag.
    struct Drawable
    {
    };
    struct Cat
    {
    };
    struct Dog
    {
    };

    em.attach<Drawable>(e1);
    em.attach<Drawable>(e2);
    em.attach<Drawable>(e5);

    for (EntityID e : EntityCapture<Drawable>(em))
    {
        if ((e != e1) && (e != e2) && (e != e5))
        {
            CHECK(false);
        }
    }

    em.detach<Drawable>(e1);
    em.detach<Drawable>(e2);
    em.detach<Drawable>(e5);

    for (EntityID e : EntityCapture<Drawable>(em))
    {
        CHECK(false);
    }

    em.attach<Cat>(e4);
    em.attach<Dog>(e4);

    for (EntityID e : EntityCapture<Cat, Dog>(em))
    {
        CHECK(e == e4);
    }

    for (EntityID e : EntityCapture<Cat>(em))
    {
        CHECK(e == e4);
    }

    for (EntityID e : EntityCapture<Dog>(em))
    {
        CHECK(e == e4);
    }

    em.detach<Dog>(e4);
    for (EntityID e : EntityCapture<Dog>(em))
    {
        CHECK(false);
    }

    for (EntityID e : EntityCapture<Cat, Dog>(em))
    {
        CHECK(false);
    }

    em.detach<Cat>(e4);
    for (EntityID e : EntityCapture<Dog>(em))
    {
        CHECK(false);
    }
}
