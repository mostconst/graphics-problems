add_library(
utils

"camera.cpp"
"ArrayBuffer.cpp"
"VertexArray.cpp"
"MovableHandle.cpp"
"math_util.cpp"
"shader_s.cpp"
"Shader.cpp"
"ShaderProgramGuard.cpp"
"Color.cpp"
"Vertex.h"
"GeometryObject.cpp"
"UserContext.cpp"
)

if(GRAPHICS_PROBLEMS_BUILD_TESTS)
	add_executable(
	  camera_test
	  camera_test.cpp
	)
	target_link_libraries(
	  camera_test
	  utils
	  GTest::gtest_main
	)

	include(GoogleTest)
	gtest_discover_tests(camera_test)
endif()