file(GLOB VENDORS_SOURCES ../external/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Headers/*.hpp)
file(GLOB PROJECT_SOURCES
Sources/main.cpp
Sources/utils.h
Sources/utils.cpp
)
file(GLOB PROJECT_CONFIGS ../CMakeLists.txt
                         ../.gitattributes
                         ../.gitignore
                         ../.gitmodules)                

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("Textures" FILES ${PROJECT_TEXTURES})

set(shadersList
    "GourandBlinn.vert"
    "Gourand.frag"
    "ScreenQuad.vert"
    "ScreenQuad.frag"
    "fxaa.frag"
    "SMAA.h"
    "shaderUtils.h"
    "SMAAEdgeDetection.vert"
    "SMAALumaEdgeDetection.frag"
    "SMAAEdgeDetectionInclude.vert"
    "SMAAEdgeDetectionInclude.frag"
    "SMAABlendingWeightCalculation.vert"
    "SMAABlendingWeightCalculation.frag"
    "SMAABlendingWeightCalculationInclude.vert"
    "SMAABlendingWeightCalculationInclude.frag"
)

set(shaderSources)
foreach(text IN LISTS shadersList)
    list(APPEND shaderSources ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/${text})
endforeach()

set(shadersComplete)
foreach(text IN LISTS shadersList)
    list(APPEND shadersComplete ${CMAKE_CURRENT_BINARY_DIR}/${text})
endforeach()

add_custom_command(OUTPUT ${shadersComplete}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Shaders ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND simplecpp ${CMAKE_CURRENT_BINARY_DIR}/"SMAAEdgeDetectionInclude.vert" > ${CMAKE_CURRENT_BINARY_DIR}/"SMAAEdgeDetectionComplete.vert"
    COMMAND simplecpp ${CMAKE_CURRENT_BINARY_DIR}/"SMAAEdgeDetectionInclude.frag" > ${CMAKE_CURRENT_BINARY_DIR}/"SMAAEdgeDetectionComplete.frag"
    COMMAND simplecpp ${CMAKE_CURRENT_BINARY_DIR}/"SMAABlendingWeightCalculationInclude.vert" > ${CMAKE_CURRENT_BINARY_DIR}/"SMAABlendingWeightCalculationComplete.vert"
    COMMAND simplecpp ${CMAKE_CURRENT_BINARY_DIR}/"SMAABlendingWeightCalculationInclude.frag" > ${CMAKE_CURRENT_BINARY_DIR}/"SMAABlendingWeightCalculationComplete.frag"
    DEPENDS ${shaderSources}
)

set(EXEC_NAME smaa)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${EXEC_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${shadersComplete} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${EXEC_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} utils)
set_target_properties(${EXEC_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC_NAME})