add_executable(breakout_game main.cc)

add_subdirectory(ecs)

target_link_libraries(breakout_game PUBLIC sfml-graphics sfml-window ecs)
