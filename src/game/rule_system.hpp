#ifndef RULE_SYSTEM_H
#define RULE_SYSTEM_H


#include <memory>

#include <system.hpp>

namespace breakout::systems {
    class rule_system : public system {
        std::shared_ptr<sf::RenderWindow> window;
    public:
      rule_system(std::shared_ptr<sf::RenderWindow> window)
          : window(window) {}

      void process(world_type &world) override;
    };
}  // namespace breakout::systems

#endif /* RULE_SYSTEM_H */
