add_library(testing_tool
    SnapshotChecker.h
    SnapshotChecker.cpp)
target_link_libraries(testing_tool PUBLIC utils)
target_include_directories(testing_tool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(GRAPHICS_PROBLEMS_BUILD_TESTS)
    add_subdirectory(tests)
endif()