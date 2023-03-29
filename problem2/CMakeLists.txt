file(GLOB VENDORS_SOURCES ../external/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Headers/*.hpp)
file(GLOB PROJECT_SOURCES
Sources/main.cpp
Sources/tesselate.cpp
Sources/helper.cpp
)
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

set(EXEC_NAME problem2)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${EXEC_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${EXEC_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} utils)
set_target_properties(${EXEC_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC_NAME})

if(GRAPHICS_PROBLEMS_BUILD_TESTS)
	add_executable(
	  tesselate_test
	  Sources/tesselate_test.cpp
	)
	target_link_libraries(
	  tesselate_test
	  utils
	  GTest::gtest_main
	)

	include(GoogleTest)
	gtest_discover_tests(tesselate_test)
endif()

add_custom_command(
    TARGET ${EXEC_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${EXEC_NAME}/Shaders $<TARGET_FILE_DIR:${EXEC_NAME}>
    DEPENDS ${PROJECT_SHADERS})