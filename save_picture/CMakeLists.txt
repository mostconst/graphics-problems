file(GLOB PROJECT_CONFIGS ../CMakeLists.txt
                         ../.gitattributes
                         ../.gitignore
                         ../.gitmodules)

set(EXEC_NAME save_picture)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_library(save_picture_core
    Sources/utils.cpp
    Sources/utils.h
)
target_link_libraries(save_picture_core glad)
target_include_directories(save_picture_core PUBLIC Sources)

add_executable(${EXEC_NAME}
    Sources/main.cpp
    ${PROJECT_CONFIGS}
)
target_link_libraries(${EXEC_NAME} glfw utils save_picture_core)
set_target_properties(${EXEC_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC_NAME})

if(GRAPHICS_PROBLEMS_BUILD_TESTS)
    add_subdirectory(tests)
endif()