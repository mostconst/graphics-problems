add_executable(
  hello_test
  hello_test.cc
  SnapshotChecker.h
  SnapshotChecker.cpp
  ${VENDORS_SOURCES}
)
target_link_libraries(
  hello_test
  glfw
  ${GLFW_LIBRARIES}
  ${GLAD_LIBRARIES}
  utils
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)