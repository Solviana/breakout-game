#ifndef WORLD_H
#define WORLD_H

#include "components.hpp"
#include "ecs.hpp"

extern template class ecs<position, velocity, reflector, lives, bounding_box, control,
					deadly, sf::RectangleShape, sf::CircleShape>;

namespace breakout {
    using world_type = ecs<position, velocity, reflector, lives, bounding_box,
			   control, deadly, sf::RectangleShape, sf::CircleShape>;

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
