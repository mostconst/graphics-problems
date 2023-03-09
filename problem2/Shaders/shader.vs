#version 330 core
layout (location = 0) in vec3 localPosition;
layout (location = 1) in vec3 vertexColorInput;

out vec3 vertexColor;

uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(localPosition, 1.0f);
	vertexColor = vertexColorInput;
}