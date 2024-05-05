#ifndef SYSTEM_H
#define SYSTEM_H

#include <world.hpp>

namespace breakout {
    class system {
    public:
        virtual void process(world_type& world) = 0;
        virtual ~system() = default;
    };
} // namespace breakout
#endif /* SYSTEM_H */
