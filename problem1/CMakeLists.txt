file(GLOB VENDORS_SOURCES ../external/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Headers/*.hpp)
file(GLOB PROJECT_SOURCES Sources/*.cpp)
file(GLOB PROJECT_SHADERS Shaders/*.comp
                          Shaders/*.frag
                          Shaders/*.geom
                          Shaders/*.vert
                          Shaders/*.fs
                          Shaders/*.vs)
file(GLOB PROJECT_CONFIGS ../CMakeLists.txt
                         ../.gitattributes
                         ../.gitignore
                         ../.gitmodules)                

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("Textures" FILES ${PROJECT_TEXTURES})

set(EXEC_NAME problem1)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${EXEC_NAME} ${PROJECT_SOURCES} Sources/camera.cpp ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${EXEC_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${EXEC_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC_NAME})

add_executable(
  camera_test
  camera_test.cpp
)
target_link_libraries(
  camera_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(camera_test)

add_custom_command(
    TARGET ${EXEC_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${EXEC_NAME}/Shaders $<TARGET_FILE_DIR:${EXEC_NAME}>
    DEPENDS ${PROJECT_SHADERS})