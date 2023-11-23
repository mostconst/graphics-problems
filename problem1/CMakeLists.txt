file(GLOB PROJECT_SOURCES
Sources/main.cpp
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

source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Textures" FILES ${PROJECT_TEXTURES})

set(EXEC_NAME problem1)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${EXEC_NAME} ${PROJECT_SOURCES}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS})
target_link_libraries(${EXEC_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} utils)
set_target_properties(${EXEC_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC_NAME})

add_custom_command(
    TARGET ${EXEC_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${EXEC_NAME}/Shaders $<TARGET_FILE_DIR:${EXEC_NAME}>
    DEPENDS ${PROJECT_SHADERS})