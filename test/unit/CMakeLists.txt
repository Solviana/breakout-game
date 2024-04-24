add_executable(test_entity test_entity.cc)
target_link_libraries(test_entity GTest::gtest GTest::gtest_main ecs)
add_test(NAME entity COMMAND test_entity)
