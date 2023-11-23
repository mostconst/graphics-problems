add_library(testing_tool
    SnapshotChecker.h
    SnapshotChecker.cpp
    Image.h
    Image.cpp
    ImageDimensions.h)
target_include_directories(testing_tool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(testing_tool PRIVATE ${GRAPHICS_PROBLEMS_STB_INCLUDE_DIR})
if(GRAPHICS_PROBLEMS_BUILD_TESTS)
    add_subdirectory(tests)
endif()