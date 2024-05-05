#include "game_manager.hpp"

using namespace std::literals::string_literals;

namespace breakout {

    namespace {
        sf::Text build_pause_message() {
            static sf::Font font; // this has to be static because text only takes a reference, not a copy
            // todo platform independent todo make font a singleton or something to avoid reloading a font multiple times
            if(!font.loadFromFile("/usr/share/fonts/truetype/fantasque-sans/Normal/TTF/FantasqueSansMono-Regular.ttf")) {
                throw std::runtime_error("Failed to load font");
            }
            sf::Text text{"Press space to start"s, font, 50};

            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
            text.setPosition(constants::window_width/2.0f, constants::window_height/2.0f);
            text.setFillColor(sf::Color::White);
            return text;
        }
    } // unnamed namespace

    void game_manager::play() {
        breakout::world_type& world = breakout::world::get();
        auto pause_message = build_pause_message();

        window->setFramerateLimit(60);      // Max rate is 60 frames per second

        while (window->isOpen()) {
            handle_events();

            if(!is_paused) {
                for (auto &sys : systems) {
                    sys->process(world);
                }
            } else {
                window->draw(pause_message);
            }

            // Display the updated graphics
            window->display();
        }
    }

    void game_manager::handle_events() {
        sf::Event event{};
        while (window->pollEvent(event)) {
            switch(event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Key::Space:
                    is_paused = !is_paused;
                    break;
                case sf::Keyboard::Key::Q:
                    window->close();
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
    }

}
