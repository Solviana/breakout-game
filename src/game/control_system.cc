#include <control_system.hpp>

// todo move this into a singleton or something
namespace breakout::systems {

    using action = std::function<void(world_type::entity)>;
    // todo refactor
    // keys -> action if pressed, action if released
    std::unordered_map<sf::Keyboard::Key, std::pair<action,action>> keymap{
        {
            sf::Keyboard::Key::Left,
                {
                    [](world_type::entity ent) -> void {
                        if(auto* p = ent.get<velocity>(); p) {
                            *p = sf::Vector2f{-10.0, 0.0}; // todo constants
                        }
                    },

                    [](world_type::entity ent) -> void {
                        if(auto* p = ent.get<velocity>(); p) {
                            *p = sf::Vector2f{0.0, 0.0};
                        }
                    }
               }
        },
        {
            sf::Keyboard::Key::Right,
            {
                [](world_type::entity ent) -> void {
                    if(auto* p = ent.get<velocity>(); p) {
                        *p = sf::Vector2f{10.0, 0.0};
                    }
                },

                [](world_type::entity ent) -> void {
                    if(auto* p = ent.get<velocity>(); p) {
                        *p = sf::Vector2f{0.0, 0.0};
                    }
                }
            }
        }
    };

    void control_system::process(world_type& world) {
        for(auto ent : world) {
            if (auto* ctrl = ent.get<control>(); ctrl) {
                for(auto key: ctrl->keys) {
                    // todo make this event based
                    if (sf::Keyboard::isKeyPressed(key)) {
                        keymap[key].first(ent);
                        break; // omfg wtf
                    }
                    else {
                        keymap[key].second(ent);
                    }
                }
            }
        }
    }

} // namespace breakout::systems
