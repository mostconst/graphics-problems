#version 330 core

uniform sampler2D previousLayerDepth;

in vec4 vertexColor;

out vec4 fragmentColor;

void main()
{
	vec3 fragmentTextureCoordinate = gl_FragCoord.xyz  / vec3(textureSize(previousLayerDepth, 0), 1.0);
	float fragmentPreviousDepth = texture(previousLayerDepth, fragmentTextureCoordinate.xy).x;
	float fragmentDepth = gl_FragCoord.z;
	if (fragmentDepth <= fragmentPreviousDepth)
		discard;
    fragmentColor = vertexColor;
}