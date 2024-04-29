#ifndef CONTROL_SYSTEM_H
#define CONTROL_SYSTEM_H

#include <memory>

#include <system.hpp>

namespace breakout::systems {
    class control_system : public system {
	std::shared_ptr<sf::RenderWindow> window;
    public:
      control_system(std::shared_ptr<sf::RenderWindow> window)
          : window(window) {}

      void process(world_type &world) override;
    };
}  // namespace breakout::systems
#endif /* CONTROL_SYSTEM_H */
