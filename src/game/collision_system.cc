#include <collision_system.hpp>

namespace breakout::systems {
    namespace {
        enum class collision_direction { left, top, right, bottom };

        // collection of messed up collision logic
        struct collision_utility {

            static bool is_reflector(world_type::entity e) {
                return (e.has<reflector>() || e.has<velocity_reflector>() || e.has<distance_reflector>()) &&
                    e.has<position, bounding_box>();
            }

            static bool is_colliding(world_type::entity& e1, world_type::entity& e2) {
                auto* pos1 = e1.get<position>();
                auto* pos2 = e2.get<position>();
                auto* bbox1 = e1.get<bounding_box>();
                auto* bbox2 = e2.get<bounding_box>();
                bool boxes_exist = pos1 && pos2 && bbox1 && bbox2; // NOLINT
                return boxes_exist && // nullptr check
                    sf::FloatRect{*pos1, *bbox1}.intersects(sf::FloatRect{*pos2, *bbox2});
            }

            static collision_direction get_side(world_type::entity& e1, world_type::entity& e2) {
                collision_direction side; // side refers to the side of e2 that is in collision
                auto* pos1 = e1.get<position>();
                auto* pos2 = e2.get<position>();
                auto* bbox1 = e1.get<bounding_box>();
                auto* bbox2 = e2.get<bounding_box>();
                // jesus...
                float left_overlap = pos1->x + bbox1->x - pos2->x;
                float right_overlap = pos2->x + bbox2->x - pos1->x;
                float top_overlap = pos1->y + bbox1->y - pos2->y;
                float bottom_overlap = pos2->y + bbox2->y - pos1->y;
                bool from_left = std::abs(left_overlap) < std::abs(right_overlap);
                bool from_top = std::abs(top_overlap) < std::abs(bottom_overlap);
                float min_x_overlap = from_left ? left_overlap : right_overlap;
                float min_y_overlap = from_top ? top_overlap : bottom_overlap;
                if (std::abs(min_x_overlap) < std::abs(min_y_overlap)) {
                    if (from_left) {
                        side = collision_direction::left;
                    }
                    else {
                        side = collision_direction::right;
                    }
                }
                else {
                    if (from_top) {
                        side = collision_direction::top;
                    }
                    else {
                        side = collision_direction::bottom;
                    }
                }
                return side;
            }

            static void invert_direction(world_type::entity& e1, world_type::entity& e2, collision_direction side) {
                switch (side) {
                case collision_direction::left:
                    if (e1.has<velocity>() && e2.has<reflector>()) {
                        auto *vel = e1.get<velocity>();
                        vel->x = vel->x >= 0 ? -vel->x : vel->x; // force moving left
                    }
                    break;
                case collision_direction::right:
                    if (e1.has<velocity>() && e2.has<reflector>()) {
                        auto *vel = e1.get<velocity>();
                        vel->x = vel->x >= 0 ? vel->x : -vel->x; // force moving right
                    }
                    break;
                case collision_direction::top:
                    if (e1.has<velocity>() && e2.has<reflector>()) {
                        auto *vel = e1.get<velocity>();
                        vel->y = vel->y >= 0 ? -vel->y : vel->y; // force moving up
                    }
                    break;
                case collision_direction::bottom:
                    if (e1.has<velocity>() && e2.has<reflector>()) {
                        auto *vel = e1.get<velocity>();
                        vel->y = vel->y >= 0 ? vel->y : -vel->y; // force moving down
                    }
                    break;
                }
            }
            static void transfer_speed(world_type::entity& e1, world_type::entity& e2) {
                auto* v1 = e1.get<velocity>();
                auto* v2 = e2.get<velocity>();
                if(v1 != nullptr && v2 != nullptr) {
                    velocity temp = *v2;
                    if(e1.has<velocity_reflector>()) {
                        *v2 += *v1 * constants::velocity_reflect_factor;
                    }
                }
            }

            static void distance_speedup(world_type::entity& e1, world_type::entity& e2) {
                auto* pos1 = e1.get<position>();
                auto* pos2 = e2.get<position>();
                auto* bbox1 = e1.get<bounding_box>();
                auto* bbox2 = e2.get<bounding_box>();

                float distance = (pos1->x + bbox1->x / 2) - (pos2->x + bbox2->x / 2);

                if (e2.has<distance_reflector>() && e1.has<velocity>()) {
                    auto* v1 = e1.get<velocity>();
                    *v1 += distance * velocity{constants::distance_reflect_factor,0.0};
                }
            }

            static void reflect_velocity(world_type::entity& e1, world_type::entity& e2, collision_direction side) {
                invert_direction(e1, e2, side);
                transfer_speed(e1, e2);
                distance_speedup(e1, e2);
            }

            static void handle_effects(world_type::entity& e1, world_type::entity& e2) {
                if (auto* l = e1.get<lives>(); l != nullptr) {
                    l->count--;
                    if(l->count <= 0) {
                        e1.kill();
                    }
                }
                if (auto* l = e2.get<lives>(); l != nullptr) {
                    l->count--;
                    if(l->count <= 0) {
                        e2.kill();
                    }
                }
                if(e1.has<deadly>()) {
                    e2.kill();
                }
                if (e2.has<deadly>()) {
                    e1.kill();
                }
            }

        };
    } // unnamed namespace

    void collision_system::process(world_type& world) {
        for(auto ent1 : world) {
            if(ent1.has<position, velocity, bounding_box>()) {
                for (auto ent2 : world) {
                    if(collision_utility::is_reflector(ent2) && ent2.id() != ent1.id()
                       && ent1.is_alive() && collision_utility::is_colliding(ent1, ent2)) {
                        auto side = collision_utility::get_side(ent1, ent2);
                        collision_utility::reflect_velocity(ent1, ent2, side);
                        collision_utility::handle_effects(ent1, ent2);
                    }
                }
            }
        }
    }
} // namespace breakout::systems
