add_library(game world.cc render_system.cc dynamics_system.cc collision_system.cc control_system.cc rule_system.cc)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(game PUBLIC sfml::sfml ecs)
