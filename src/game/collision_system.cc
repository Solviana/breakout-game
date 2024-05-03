#include <collision_system.hpp>

namespace breakout::systems {
    void collision_system::process(world_type& world) {
	for(auto ent1 : world) {
	    if(ent1.has<position, velocity, bounding_box>()) {
		for (auto ent2 : world) {
		    if(ent2.has<reflector, position, bounding_box>() && ent2.id() != ent1.id() && ent1.is_alive()) {
			this->process_collision(ent1, ent2);
		    }
		}
	    }
	}
    }

    void collision_system::process_collision(world_type::entity& e1, world_type::entity& e2) {
	auto* pos1 = e1.get<position>();
	auto* pos2 = e2.get<position>();
	auto* bbox1 = e1.get<bounding_box>();
	auto* bbox2 = e2.get<bounding_box>();
	sf::FloatRect box1{*pos1, *bbox1}; // todo unsafe
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
	    if (e2.has<deadly>()) {
		e1.kill();
	    }
	}
    }
} // namespace breakout::systems
