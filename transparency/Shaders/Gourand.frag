#version 330 core

uniform sampler2DShadow previousLayerDepth;

in vec4 vertexColor;

out vec4 fragmentColor;

void main()
{
	if (gl_FragCoord.z < texture(previousLayerDepth, vec3(gl_FragCoord.xy, 1.0)))
		discard;
    fragmentColor = vertexColor;
}