add_executable(testing_tool_test testing_tool_test.cpp)
target_link_libraries(
  testing_tool_test
  testing_tool
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testing_tool_test)