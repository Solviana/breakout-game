#ifndef SYSTEM_H
#define SYSTEM_H

#include <world.hpp>

namespace breakout::systems {
    class system {
    public:
	virtual void process(breakout::world_type& world) = 0;
    };
} // namespace breakout::systems
#endif /* SYSTEM_H */
