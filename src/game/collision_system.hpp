#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <system.hpp>

namespace breakout::systems {
    class collision_system : public system {

    public:
        void process(world_type& world) override;
    };
} // namespace breakout::systems

#endif /* COLLISION_SYSTEM_H */
