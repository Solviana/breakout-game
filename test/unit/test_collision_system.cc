#include <gtest/gtest.h>
#include <algorithm>

#include <collision_system.hpp>

using namespace std::literals::string_literals;
using namespace breakout;

struct collision_test : testing::Test {
    world_type& world {world::get()};
    world_type::entity e1{world.add_entity()};
    world_type::entity e2{world.add_entity()};
    systems::collision_system sys;

    void SetUp() override {
    }

    void TearDown() override {
        for (auto e : world) {
            e.kill();
        }
    }
};

TEST_F(collision_test, top_collision) {
    e1.add(position{0.0f, 0.0f},
           velocity{1.0f, 1.0f},
           bounding_box{20.0,20.0},
           reflector{});

    e2.add(position{0.0f, 19.0f},
           velocity{1.0f, -1.0f},
           bounding_box{100.0,20.0},
           reflector{});
    sys.process(world);
    // in case of top collision only the x coordinate is modified
    EXPECT_FLOAT_EQ(e1.get<velocity>()->x, 1.0f);
    EXPECT_FLOAT_EQ(e1.get<velocity>()->y, -1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->x, 1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->y, 1.0f);
}


TEST_F(collision_test, left_collision) {
    e1.add(position{0.0f, 0.0f},
           velocity{1.0f, 1.0f},
           bounding_box{20.0,20.0},
           reflector{});

    e2.add(position{19.0f, 0.0f},
           velocity{-1.0f, -1.0f},
           bounding_box{100.0,20.0},
           reflector{});
    sys.process(world);
    // in case of top collision only the x coordinate is modified
    EXPECT_FLOAT_EQ(e1.get<velocity>()->x, -1.0f);
    EXPECT_FLOAT_EQ(e1.get<velocity>()->y, 1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->x, 1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->y, -1.0f);
}

TEST_F(collision_test, right_collision) {
    e1.add(position{99.0f, 0.0f},
           velocity{-1.0f, 1.0f},
           bounding_box{20.0,20.0},
           reflector{});

    e2.add(position{0.0f, 0.0f},
           velocity{1.0f, -1.0f},
           bounding_box{100.0,20.0},
           reflector{});
    sys.process(world);
    // in case of top collision only the x coordinate is modified
    EXPECT_FLOAT_EQ(e1.get<velocity>()->x, 1.0f);
    EXPECT_FLOAT_EQ(e1.get<velocity>()->y, 1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->x, -1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->y, -1.0f);
}

TEST_F(collision_test, bottom_collision) {
    e1.add(position{0.0f, 19.0f},
           velocity{1.0f, -1.0f},
           bounding_box{20.0,20.0},
           reflector{});

    e2.add(position{0.0f, 0.0f}, velocity{-1.0f, 1.0f},
           bounding_box{100.0, 20.0}, reflector{});
    sys.process(world);
    // in case of top collision only the x coordinate is modified
    EXPECT_FLOAT_EQ(e1.get<velocity>()->x, 1.0f);
    EXPECT_FLOAT_EQ(e1.get<velocity>()->y, 1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->x, -1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->y, -1.0f);
}

TEST_F(collision_test, velocity_reflector) {
    e1.add(position{0.0f, 0.0f},
           velocity{1.0f, -1.0f},
           bounding_box{20.0,20.0},
           velocity_reflector{});

    e2.add(position{0.0f, 19.0f}, velocity{-1.0f, 1.0f},
           bounding_box{100.0, 20.0}, velocity_reflector{});
    sys.process(world);
    // in case of top collision only the x coordinate is modified
    // limitation EXPECT_FLOAT_EQ(e1.get<velocity>()->x, (1.0 - constants::velocity_reflect_factor) * 1.0f);
    // limitation EXPECT_FLOAT_EQ(e1.get<velocity>()->y, (1.0 - constants::velocity_reflect_factor) * -1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->x, (1.0 - constants::velocity_reflect_factor) * -1.0f);
    EXPECT_FLOAT_EQ(e2.get<velocity>()->y, (1.0 - constants::velocity_reflect_factor) * 1.0f);
}
