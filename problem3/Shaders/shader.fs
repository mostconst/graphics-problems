#version 330 core
out vec4 fragmentColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    fragmentColor = vec4(objectColor * lightColor, 1.0f);
}