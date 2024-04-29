add_executable(breakout_game main.cc)

add_subdirectory(ecs)
add_subdirectory(game)

target_link_libraries(breakout_game PUBLIC sfml-graphics sfml-window game)
