#ifndef DYNAMICS_SYSTEM_H
#define DYNAMICS_SYSTEM_H

#include <system.hpp>

namespace breakout::systems {
    class dynamics_system : public system {
    public:
	void process(world_type& sworld) override;
    };
} // namespace breakout::systems

#endif /* DYNAMICS_SYSTEM_H */
