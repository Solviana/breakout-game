#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <system.hpp>

namespace breakout::systems {
    class collision_system : public system {
	void process_collision(world_type::entity &e1, world_type::entity &e2);

    public:
	void process(world_type& world) override;
    };
} // namespace breakout::systems

#endif /* COLLISION_SYSTEM_H */
