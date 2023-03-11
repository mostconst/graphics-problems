#version 330 core
layout (location = 0) in vec3 localPositionFirstFrame;
layout (location = 1) in vec3 vertexColorInput;
layout (location = 2) in vec3 localPositionSecondFrame;

out vec3 vertexColor;

uniform mat4 mvpMatrix;
uniform float morphCoeff;

void main()
{
	vec3 resultPosition = localPositionFirstFrame * (1.0f - morphCoeff) + localPositionSecondFrame * morphCoeff;
	gl_Position = mvpMatrix * vec4(resultPosition, 1.0f);
	vertexColor = vertexColorInput;
}