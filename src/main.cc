// Code for a "breakout" game
// Based on a talk by Vittorio Romeo
// Uses the SFML graphics library
#include <entity.hpp>

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <set>
#include <unordered_map>

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Code for a "breakout" game
// Based on a talk by Vittorio Romeo
// Uses the SFML graphics library

struct constants {
  static constexpr int window_width{1024};
  static constexpr int window_height{768};
};

#endif // CONSTANTS_H

using namespace std::literals;

struct position : public sf::Vector2f {using sf::Vector2f::Vector2;}; // inherit constructors

struct velocity : public sf::Vector2f {using sf::Vector2f::Vector2;};

struct reflector {};

struct lives {int count;};

struct deadly{};

struct control {std::set<sf::Keyboard::Key> keys;};

struct bounding_box : public sf::Vector2f {using sf::Vector2f::Vector2f;};

using world_type = ecs::entity_table<position, velocity, reflector, lives, bounding_box, control,
                                     deadly, sf::RectangleShape, sf::CircleShape>;

class renderSystem {
    sf::RenderWindow& window; // unsafe AF

  public:
    explicit renderSystem(sf::RenderWindow& window) : window(window) {}

    void process(world_type& world) {
        for(auto ent : world) {
            if (ent.has<position>()) {
                auto *pos = ent.get<position>();
                /*if (ent.has<sf::Sprite>()){
                    auto* sprite = ent.get<sf::Sprite>();
                    sprite->setPosition(pos->x, pos->y);
                    window.draw(*sprite);
                    }*/

                if (ent.has<sf::RectangleShape>()){
                    auto* shape = ent.get<sf::RectangleShape>();
                    shape->setPosition(pos->x, pos->y);
                    window.draw(*shape);
                }

                if (ent.has<sf::CircleShape>()){
                    auto* shape = ent.get<sf::CircleShape>();
                    shape->setPosition(pos->x, pos->y);
                    window.draw(*shape);
                }
            }
        }
    }
};

class moveSystem {
public:
    void process(world_type& world) {
        for(auto ent : world) {
            if(ent.has<position, velocity>()) {
                auto* pos = ent.get<position>();
                auto* vel = ent.get<velocity>();
                *pos += *vel;
            }
        }
    }
};

class collisionSystem { // todo refactor..

    void process_collision(world_type::entity& e1, world_type::entity& e2) {
        auto* pos1 = e1.get<position>();
        auto* pos2 = e2.get<position>();
        auto* bbox1 = e1.get<bounding_box>();
        auto* bbox2 = e2.get<bounding_box>();
        sf::FloatRect box1{*pos1, *bbox1};
        sf::FloatRect box2{*pos2, *bbox2};

        if (box1.intersects(box2)) {
            float left_overlap = pos1->x + bbox1->x - pos2->x;
            float right_overlap = pos2->x + bbox2->x - pos1->x;
            float top_overlap = pos1->y + bbox1->y - pos2->y;
            float bottom_overlap = pos2->y + bbox2->y - pos1->y;
            // Fuckery
            bool from_left = std::abs(left_overlap) < std::abs(right_overlap);
            bool from_top = std::abs(top_overlap) < std::abs(bottom_overlap);

            // Use the right or left overlap as appropriate
            float min_x_overlap = from_left ? left_overlap : right_overlap;
            float min_y_overlap = from_top ? top_overlap : bottom_overlap;

            // If the ball hit the left or right side of the brick, change its horizontal direction
            // If the ball hit the top or bottom of the brick, change its vertical direction
            if (std::abs(min_x_overlap) < std::abs(min_y_overlap)) {

                if (from_left) {
                    if (e1.has<velocity>()) {
                        auto *vel = e1.get<velocity>();
                        vel->x = vel->x >= 0 ? -vel->x : vel->x; // force moving left
                    }
                    if (e2.has<velocity>()) {
                        auto *vel = e2.get<velocity>();
                        vel->x = vel->x >= 0 ? vel->x : -vel->x; // force moving right
                    }
                } else {
                    if (e1.has<velocity>()) {
                        auto *vel = e1.get<velocity>();
                        vel->x = vel->x >= 0 ? vel->x : -vel->x; // force moving right
                    }
                    if (e2.has<velocity>()) {
                        auto *vel = e2.get<velocity>();
                        vel->x = vel->x >= 0 ? -vel->x : vel->x; // force moving left
                    }
                }
            }
            else {

                if (from_top) {
                    if (e1.has<velocity>()) {
                        auto *vel = e1.get<velocity>();
                        vel->y = vel->y >= 0 ? -vel->y : vel->y; // force moving up
                    }
                    if (e2.has<velocity>()) {
                        auto *vel = e2.get<velocity>();
                        vel->x = vel->x >= 0 ? vel->y : -vel->y; // force moving down
                    }
                } else {
                    if (e1.has<velocity>()) {
                        auto *vel = e1.get<velocity>();
                        vel->y = vel->y >= 0 ? vel->y : -vel->y; // force moving down
                    }
                    if (e2.has<velocity>()) {
                        auto *vel = e2.get<velocity>();
                        vel->x = vel->x >= 0 ? -vel->y : vel->y; // force moving up
                    }
                }
            }
            if (auto* l = e1.get<lives>(); l != nullptr) {
                l->count--;
                if(l->count <= 0) {
                    e1.kill();
                }
            }
            if (auto* l = e2.get<lives>(); l != nullptr) {
                l->count--;
                if(l->count <= 0) {
                    e2.kill();
                }
            }
            if(e1.has<deadly>()) {
                e2.kill();
            }
            if(e2.has<deadly>()) {
                e1.kill();
            }
        }
    }

public:

    void process(world_type& world) {
        for(auto ent1 : world) {
            if(ent1.has<position, velocity, bounding_box>()) {
                for (auto ent2 : world) {
                    if(ent2.has<reflector, position, bounding_box>()) {
                        this->process_collision(ent1, ent2);
                    }
                }
            }
        }
    }
};

class controlSystem {
    sf::RenderWindow& window;

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

public:
    controlSystem(sf::RenderWindow &window): window(window) {}

    void process(world_type& world) {
        for(auto ent : world) {
            if (auto* ctrl = ent.get<control>(); ctrl) {
                for(auto key: ctrl->keys) {
                    if (sf::Keyboard::isKeyPressed(key)) {
                        std::cout << "pressing key " << key << std::endl;
                        keymap[key](ent);
                    }
                }
            }
        }
    }
};

void level1(world_type& world) {
    for (auto ent : world) {
        ent.kill();
    }
    auto ball_sprite = sf::CircleShape{10.0};
    ball_sprite.setFillColor(sf::Color::Blue);

    auto paddle_sprite = sf::RectangleShape({10.0, 50.0});
    paddle_sprite.setFillColor(sf::Color::Blue);

    auto boundary_sprite_vertical =
        sf::RectangleShape({10.0, constants::window_height});
    boundary_sprite_vertical.setFillColor(sf::Color::White);

    auto boundary_sprite_horizontal =
        sf::RectangleShape({constants::window_width, 10.0});
    boundary_sprite_vertical.setFillColor(sf::Color::White);

    auto paddle = world.add_entity();
    paddle.add(
               position{constants::window_width / 2, constants::window_height - 30},
               sf::RectangleShape{{100.0, 10.0}},
               bounding_box{100.0, 10.0},
               reflector{},
               control{{sf::Keyboard::Left, sf::Keyboard::Right}});

    auto ball = world.add_entity();
    ball.add(position{constants::window_width / 2, constants::window_height / 2},
             ball_sprite,
             bounding_box{20, 20},
             velocity{4.0, -4.0});

    auto edge_left = world.add_entity();
    edge_left.add(position{0.0, 0.0}, boundary_sprite_vertical,
                  bounding_box{boundary_sprite_vertical.getSize().x,
                               boundary_sprite_vertical.getSize().y},
                  reflector{});

    auto edge_right = world.add_entity();
    edge_right.add(position{constants::window_width - 10, 0.0},
                   boundary_sprite_vertical,
                   bounding_box{boundary_sprite_vertical.getSize().x,
                                boundary_sprite_vertical.getSize().y},
                   reflector{});

    auto edge_top = world.add_entity();
    edge_top.add(position{0.0, 0.0}, boundary_sprite_horizontal,
                 bounding_box{boundary_sprite_horizontal.getSize().x,
                              boundary_sprite_horizontal.getSize().y},
                 reflector{});

    auto edge_bottom = world.add_entity();
    edge_bottom.add(position{0.0, constants::window_height - 10},
                    boundary_sprite_horizontal,
                    bounding_box{boundary_sprite_horizontal.getSize().x,
                                 boundary_sprite_horizontal.getSize().y},
                    deadly{});

    const int cols = 10;
    const int rows = 4;
    const int brick_spacing = 10;
    const int brick_offset_top = 30;
    const int brick_offset_side = 20;
    const int brick_height =
        (constants::window_height / 4 - brick_offset_top - rows * brick_spacing) /
        rows;
    const int brick_width =
        (constants::window_width - cols * brick_spacing - 2 * brick_offset_side) /
        cols;
    sf::RectangleShape brick_sprite{{brick_width, brick_height}};
    brick_sprite.setFillColor(sf::Color::White);

    for (int i = 0; i < rows; i++) {
        float y = brick_offset_top + i * (brick_height + brick_spacing);
        for (int j = 0; j < cols; j++) {
            float x = brick_offset_side + j * (brick_width + brick_spacing);
            world.add_entity().add(
                                   position{x, y}, brick_sprite,
                                   bounding_box{brick_sprite.getSize().x, brick_sprite.getSize().y},
                                   reflector{}, lives{1});
        }
    }
}

// The main function for the program
int main() {
    world_type world;
    sf::RenderWindow game_window{{constants::window_width, constants::window_height},
                                 "Breakout v1"s,};
    renderSystem render_sys{game_window};
    controlSystem control_sys{game_window};
    moveSystem move_sys{};
    collisionSystem collision_sys{};

    level1(world);
    // Limit the framerate
    // This allows other processes to run and reduces power consumption
    game_window.setFramerateLimit(60);      // Max rate is 60 frames per second

    // Game loop
    // Clear the screen
    // Check for new events
    // Calculate the updated graphics
    // Display the updated graphics
    while (game_window.isOpen()) {
        // Clear the screen
        game_window.clear(sf::Color::Black);
        // Check for any events since the last loop iteration
        sf::Event event{};

        // If the user pressed "Escape", or clicked on "close", we close the window
        // This will terminate the program
        while (game_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                game_window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            std::cout << "quitting..." << std::endl;
        }

        // Calculate the updated graphics
        // This space left intentionally blank!
        move_sys.process(world);
        control_sys.process(world);
        collision_sys.process(world);
        render_sys.process(world);
        // Display the updated graphics
        game_window.display();
    }
}
