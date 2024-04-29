#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <memory>

#include <system.hpp>
#include <world.hpp>

namespace breakout::systems {
    class render_system : public system {
	std::shared_ptr<sf::RenderWindow> window;

    public:
	explicit render_system(std::shared_ptr<sf::RenderWindow> window)
	    : window(window) {}

	void process(world_type &world) override;
    };
} // namespace systems

#endif /* RENDER_SYSTEM_H */
