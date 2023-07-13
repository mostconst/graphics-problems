add_executable(
  hello_test
  hello_test.cc
  ${VENDORS_SOURCES}
)
target_link_libraries(
  hello_test
  glfw
  testing_tool
  ${GLFW_LIBRARIES}
  ${GLAD_LIBRARIES}
  utils
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)