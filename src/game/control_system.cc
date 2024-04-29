#include <control_system.hpp>

// todo move this into a singleton or something
namespace breakout::systems {

    using action = std::function<void(world_type::entity)>;
    std::unordered_map<sf::Keyboard::Key, action> keymap{
	{
	    sf::Keyboard::Key::Left, [](world_type::entity ent) -> void {
		if(auto* p = ent.get<position>(); p) {
		    *p += sf::Vector2f{-10.0,0};
		}
	    }
	},
	{
	    sf::Keyboard::Key::Right, [](world_type::entity ent) -> void {
		if(auto* p = ent.get<position>(); p) {
		    *p += sf::Vector2f{10.0,0};
		}
	    }
	}
    };

    void control_system::process(world_type& world) {
	for(auto ent : world) {
	    if (auto* ctrl = ent.get<control>(); ctrl) {
		for(auto key: ctrl->keys) {
		    if (sf::Keyboard::isKeyPressed(key)) {
			keymap[key](ent);
		    }
		}
	    }
	}
    }

}
