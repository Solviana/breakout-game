#include <ecs.hpp>
#include <game.hpp>

using namespace std::string_literals;

void level1(breakout::world_type& world) {
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

    auto paddle = world.add_entity("paddle"s);
    paddle.add(
               position{constants::window_width / 2, constants::window_height - 30},
	       velocity{0.0,0.0},
               sf::RectangleShape{{100.0, 10.0}},
               bounding_box{100.0, 10.0},
               reflector{},
	       velocity_reflector{},
	       distance_reflector{},
               control{{sf::Keyboard::Left, sf::Keyboard::Right}});

    auto ball = world.add_entity("ball"s);
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
		    reflector{},
                    deadly{});

    const int cols = 10;
    const int rows = 4;
    const int brick_spacing = 30;
    const int brick_offset_top = 50;
    const int brick_offset_side = 80;
    const int brick_height =
        (constants::window_height / 3 - brick_offset_top - rows * brick_spacing) /
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
    breakout::world_type& world = breakout::world::get();
    auto game_window = std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow{{constants::window_width, constants::window_height}, "Breakout v1"s});
    breakout::systems::render_system render_sys{game_window};
    breakout::systems::control_system control_sys{game_window};
    breakout::systems::rule_system rule_sys{game_window};
    breakout::systems::dynamics_system move_sys{};
    breakout::systems::collision_system collision_sys{};

    level1(world);
    game_window->setFramerateLimit(60);      // Max rate is 60 frames per second

    while (game_window->isOpen()) {

        control_sys.process(world);
        move_sys.process(world);
        collision_sys.process(world);
	rule_sys.process(world);
        render_sys.process(world);
        // Display the updated graphics
        game_window->display();
    }
}
