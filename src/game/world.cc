#include "world.hpp"

namespace breakout {
    world_type& world::get() {
	static world_type instance{};
	return instance;
    }
} // namespace breakout::systems
