#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <SFML/Graphics.hpp>
#include <set>

struct position : public sf::Vector2f {using sf::Vector2f::Vector2f;};

struct velocity : public sf::Vector2f {
    using sf::Vector2f::Vector2f;

    velocity& operator=(const sf::Vector2f& other) {
        if (this != &other) {
            sf::Vector2f::operator=(other);
        }
        return *this;
    }
};

struct reflector {};

struct distance_reflector {};

struct velocity_reflector {};

struct lives {int count;};

struct deadly{};

struct control {std::set<sf::Keyboard::Key> keys;};

struct bounding_box : public sf::Vector2f {using sf::Vector2f::Vector2f;};

#endif /* COMPONENTS_H */
