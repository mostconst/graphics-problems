#version 330 core
layout (location = 0) in vec3 localPosition;
layout (location = 1) in vec3 localNormal;

uniform mat4 mvpMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 modelViewNormalMatrix;

out vec3 viewFragmentPosition;
out vec3 viewFragmentNormal;

void main()
{
	gl_Position = mvpMatrix * vec4(localPosition, 1.0f);
	viewFragmentPosition = vec3(modelViewMatrix * vec4(localPosition, 1.0f));
	viewFragmentNormal = normalize(modelViewNormalMatrix * localNormal);
}