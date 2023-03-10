#version 330 core
layout (location = 0) in vec3 localPosition;
layout (location = 1) in vec3 vertexColorInput;
layout (location = 2) in vec3 localPosition2;

out vec3 vertexColor;

uniform mat4 mvpMatrix;
uniform float coeff;

void main()
{
	vec3 finalPosition = localPosition * (1.0f - coeff) + localPosition2 * coeff;
	gl_Position = mvpMatrix * vec4(finalPosition, 1.0f);
	vertexColor = vertexColorInput;
}