#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "components.hpp"

struct constants {
    static constexpr int window_width{1024};
    static constexpr int window_height{768};
    static constexpr float distance_reflect_factor{0.05f};
    static constexpr float velocity_reflect_factor{0.1f};
};

#endif /* CONSTANTS_H */
