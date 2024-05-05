#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <memory>
#include <vector>

#include "system.hpp"

namespace breakout {

    class game_manager {
        std::vector<std::unique_ptr<system>> systems;
        std::shared_ptr<sf::RenderWindow> window;
        bool is_paused;
        void handle_events();
    public:
        template <typename... Systems>
        game_manager(std::shared_ptr<sf::RenderWindow> window, Systems*... args): window(window), is_paused(true) {
        // god bless https://en.cppreference.com/w/cpp/language/fold
        static_assert((... && std::is_base_of_v<system, Systems>), "All systems must be derived from breakout::system");
        (systems.emplace_back(std::unique_ptr<system>(args)), ...);
        }

        void play();
    };
} // namespace breakout

#endif /* GAME_MANAGER_H */
