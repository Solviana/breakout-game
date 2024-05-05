#ifndef WORLD_H
#define WORLD_H

#include "ecs.hpp"
#include "components.hpp"
#include "constants.hpp"

extern template class ecs<position, velocity, reflector, velocity_reflector,
                          distance_reflector, lives, bounding_box, control, brick,
                          deadly, sf::RectangleShape, sf::CircleShape>;

namespace breakout {
    using world_type = ecs<position, velocity, reflector, velocity_reflector,
                          distance_reflector, lives, bounding_box, control, brick,
                          deadly, sf::RectangleShape, sf::CircleShape>;

    class world {
    public:
        static world_type& get();
    private:
        world() {}
        world(const world_type&) = delete;
        world_type& operator=(const world_type&) = delete;
    };
}

#endif /* WORLD_H */
