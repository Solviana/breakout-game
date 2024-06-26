#include "rule_system.hpp"

using namespace std::literals::string_literals;

namespace breakout::systems {

    void rule_system::process(world_type &world) {
	// if the game is paused
        // if there is no ball respawn it
        auto ball_sprite = sf::CircleShape{10.0};
        ball_sprite.setFillColor(sf::Color::Blue);
        auto ball_it = world.find_entity("ball"s);
        if (ball_it == world.end() || !((*ball_it).is_alive())) {
            world.add_entity("ball"s). // todo clean this up
                add(position{constants::window_width / 2, constants::window_height / 2},
                    ball_sprite,
                    bounding_box{20, 20},
                    velocity{4.0, -4.0});
        }
    }

} // namespace breakout::systems
