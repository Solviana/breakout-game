#include <dynamics_system.hpp>

namespace breakout::systems {
    void dynamics_system::process(world_type& world) {
	for(auto ent : world) {
	    if (ent.has<position, velocity>()) {
		auto *pos = ent.get<position>();
		auto *vel = ent.get<velocity>();
		*pos += *vel;
	    }
	}
    }
} // namespace breakout::systems
