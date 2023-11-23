file(GLOB PROJECT_CONFIGS ../CMakeLists.txt
                         ../.gitattributes
                         ../.gitignore
                         ../.gitmodules)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_library(save_picture_core
    Sources/utils.cpp
    Sources/utils.h
)
target_link_libraries(save_picture_core glfw glad)
target_include_directories(save_picture_core PUBLIC Sources)

add_executable(malevich
    Sources/main.cpp
    ${PROJECT_CONFIGS}
)
target_link_libraries(malevich glfw save_picture_core)
set_target_properties(malevich PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/malevich)

if(GRAPHICS_PROBLEMS_BUILD_TESTS)
    add_subdirectory(tests)
endif()