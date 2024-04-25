function(declare_test)
    set(options)
    set(one_value_args
	TEST_NAME
	LINK_GTEST_MAIN
	LINK_TO_TARGET)
    set(multi_value_args
	TEST_SOURCES
	TEST_LINK_LIBRARIES)

    cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    set(TEST_EXEC_NAME "test_${ARG_TEST_NAME}")
    add_executable(${TEST_EXEC_NAME} EXCLUDE_FROM_ALL ${ARG_TEST_SOURCES})

    target_link_libraries(${TEST_EXEC_NAME} GTest::gtest ${ARG_TEST_LINK_LIBRARIES})
    if(NOT DEFINED ARG_LINK_GTEST_MAIN)
	set(ARG_LINK_GTEST_MAIN TRUE)
    endif()
    if(ARG_LINK_GTEST_MAIN)
	target_link_libraries(${TEST_EXEC_NAME} GTest::gtest_main)
    endif()

    if(ARG_LINK_TO_TARGET)
	add_dependencies(${ARG_LINK_TO_TARGET} ${TEST_EXEC_NAME})
    endif()

    add_test(NAME ${ARG_TEST_NAME} COMMAND ${TEST_EXEC_NAME})
endfunction()

add_custom_target(run_test
    COMMAND ${CMAKE_CTEST_COMMAND} -V
    COMMENT "Running tests..."
)

declare_test(
    TEST_NAME entity
    TEST_SOURCES test_entity.cc
    TEST_LINK_LIBRARIES ecs
    LINK_TO_TARGET run_test
)
