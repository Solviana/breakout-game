#include <entity.hpp>
#include <gtest/gtest.h>
#include <algorithm>

struct position {
    int x,y;
};

struct velocity {
    int vx, vy;
};

struct lives {
    int count;
};

struct entity_table_test : public testing::Test {
    ecs::entity_table<position, velocity, lives> world;
};

TEST_F(entity_table_test, empty_entity) {
    auto bob = world.add_entity();
    auto bob_pos = bob.get<position>();
    ASSERT_FALSE(bob_pos);
    ASSERT_FALSE(bob.has<position>());
}

// todo split into seperate tests for get<>, has<> etc..
TEST_F(entity_table_test, add_1_component) {
    auto bob = world.add_entity();
    bob.add(position{1,2});
    auto* bob_pos = bob.get<position>();
    ASSERT_TRUE(bob_pos);
    ASSERT_TRUE(bob.has<position>());
    ASSERT_EQ(bob_pos->x, 1);
    ASSERT_EQ(bob_pos->y, 2);

    auto* bob_velocity = bob.get<velocity>();
    ASSERT_FALSE(bob_velocity);
    ASSERT_FALSE(bob.has<velocity>());

    auto* bob_lives = bob.get<lives>();
    ASSERT_FALSE(bob_lives);
    ASSERT_FALSE(bob.has<lives>());
}

TEST_F(entity_table_test, add_2_components_single_call) {
    auto bob = world.add_entity();
    bob.add(position{1,2}, velocity{3,4});
    auto* bob_pos = bob.get<position>();
    ASSERT_TRUE(bob_pos);
    ASSERT_EQ(bob_pos->x, 1);
    ASSERT_EQ(bob_pos->y, 2);

    auto* bob_velocity = bob.get<velocity>();
    ASSERT_TRUE(bob_velocity);
    ASSERT_TRUE(bob.has<velocity>());
    ASSERT_EQ(bob_velocity->vx, 3);
    ASSERT_EQ(bob_velocity->vy, 4);

    auto* bob_lives = bob.get<lives>();
    ASSERT_FALSE(bob_lives);
    ASSERT_FALSE(bob.has<lives>());
}

TEST_F(entity_table_test, add_3_components_single_call) {
    auto bob = world.add_entity();
    bob.add(position{1,2}, velocity{3,4}, lives{5});
    auto* bob_pos = bob.get<position>();
    ASSERT_TRUE(bob_pos);
    ASSERT_EQ(bob_pos->x, 1);
    ASSERT_EQ(bob_pos->y, 2);

    auto* bob_velocity = bob.get<velocity>();
    ASSERT_TRUE(bob_velocity);
    ASSERT_EQ(bob_velocity->vx, 3);
    ASSERT_EQ(bob_velocity->vy, 4);

    auto* bob_lives = bob.get<lives>();
    ASSERT_TRUE(bob_lives);
    ASSERT_EQ(bob_lives->count, 5);
}

TEST_F(entity_table_test, variadic_has) {
    auto bob = world.add_entity();
    bob.add(position{1,2}, velocity{3,4});
    auto bob_has = bob.has<position, velocity>();
    ASSERT_TRUE(bob_has);
    bob_has = bob.has<position, lives>();
    ASSERT_FALSE(bob_has);
}

TEST_F(entity_table_test, remove_component) {
    auto bob = world.add_entity();
    bob.add(position{1,2}, velocity{3,4}, lives{5});
    ASSERT_TRUE(bob.has<position>());
    bob.remove<position>();
    ASSERT_FALSE(bob.has<position>());
}

TEST_F(entity_table_test, kill_entity) {
    auto bob = world.add_entity();
    ASSERT_TRUE(bob.is_alive());
    bob.kill();
    ASSERT_FALSE(bob.is_alive());
}

TEST_F(entity_table_test, iterate_entity) {
    /* Forward iterator unit test. */
    auto bob = world.add_entity();
    std::size_t hi = -1;
    for(auto ent : world) {
	ASSERT_EQ(ent.id(), ++hi);
    }
}

TEST_F(entity_table_test, algorithm_count) {
    /* Integration test with the std algorithm library. */
    auto bob = world.add_entity();
    auto alive_count = std::count_if(std::begin(world), std::end(world), [](auto ent){return ent.is_alive();});
    EXPECT_EQ(alive_count, 1);
}
