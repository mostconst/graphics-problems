configure_file(test_paths.h.in ${CMAKE_CURRENT_BINARY_DIR}/test_paths.h)
add_executable(
  malevich_test
  malevich_test.cpp
)
target_link_libraries(
  malevich_test
  save_picture_core
  glfw
  testing_tool
  ${GLFW_LIBRARIES}
  ${GLAD_LIBRARIES}
  utils
  GTest::gtest_main
)

target_include_directories(malevich_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

include(GoogleTest)
gtest_discover_tests(malevich_test)