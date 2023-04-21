cmake_minimum_required(VERSION 3.15)
project(graphics-problems)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(external/glfw)

option(GRAPHICS_PROBLEMS_BUILD_TESTS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(SYSTEM Glitter/Headers/
                    external/assimp/include/
                    external/glad/include/
                    external/glfw/include/
                    external/glm/
                    external/stb/
                    external/learnopengl/
					utils)

if(GRAPHICS_PROBLEMS_BUILD_TESTS)
    add_subdirectory(external/googletest)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

enable_testing()

add_subdirectory(utils)
add_subdirectory(problem1)
add_subdirectory(problem2)
add_subdirectory(problem5)
add_subdirectory(experiment)