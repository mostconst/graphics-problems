#version 330 core

#define SMAA_RT_METRICS float4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0)
#define SMAA_GLSL_3
#define SMAA_PRESET_HIGH

#define SMAA_INCLUDE_PS 0
#define SMAA_INCLUDE_VS 1

#include "SMAA.h"
#include "shaderUtils.h"

out vec2 fragmentTextureCoordinates;
out vec2 pixcoord;
out vec4 offset0;
out vec4 offset1;
out vec4 offset2;

void main()
{
    vec2 ndcPosition = triangleVertex(gl_VertexID, fragmentTextureCoordinates);
    fragmentTextureCoordinates = flipTexCoord(fragmentTextureCoordinates);

    vec4 offsets[3];
    offsets[0] = vec4(0.0, 0.0, 0.0, 0.0);
    offsets[1] = vec4(0.0, 0.0, 0.0, 0.0);
    offsets[2] = vec4(0.0, 0.0, 0.0, 0.0);
    pixcoord = vec2(0.0, 0.0);
    SMAABlendingWeightCalculationVS(fragmentTextureCoordinates, pixcoord, offsets);
    offset0 = offsets[0];
    offset1 = offsets[1];
    offset2 = offsets[2];
    gl_Position = vec4(ndcPosition.x, ndcPosition.y, 0.0, 1.0); 
}