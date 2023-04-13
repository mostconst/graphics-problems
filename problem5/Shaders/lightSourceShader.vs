#version 330 core
layout (location = 0) in vec3 localPosition;

uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(localPosition, 1.0f);
}