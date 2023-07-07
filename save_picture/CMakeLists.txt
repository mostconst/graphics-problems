file(GLOB VENDORS_SOURCES ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Headers/*.hpp)
file(GLOB PROJECT_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/Sources/main.cpp
)
file(GLOB UTIL_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/Sources/utils.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Sources/utils.h
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

set(EXEC_NAME save_picture)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${EXEC_NAME} ${PROJECT_SOURCES} ${UTIL_SOURCES} ${VENDORS_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               )
target_link_libraries(${EXEC_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} utils)
set_target_properties(${EXEC_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC_NAME})

add_custom_command(
    TARGET ${EXEC_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${EXEC_NAME}/Shaders $<TARGET_FILE_DIR:${EXEC_NAME}>
    DEPENDS ${PROJECT_SHADERS})

if(GRAPHICS_PROBLEMS_BUILD_TESTS)
    add_subdirectory(tests)
endif()