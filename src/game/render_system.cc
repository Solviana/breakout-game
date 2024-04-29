#include <render_system.hpp>

namespace breakout::systems {
    void render_system::process(world_type& world) {
	for(auto ent : world) {
	    if (auto *pos = ent.get<position>(); pos) {
		sf::Shape* shape = ent.get<sf::RectangleShape>();
		shape = shape ? shape : ent.get<sf::CircleShape>();
		if (shape) {
		    shape->setPosition(pos->x, pos->y);
		    window->draw(*shape);
		}
	    }
	}
    }
} // namespace breakout::systems
