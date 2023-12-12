//-----------------------------------------------------------------------------
// Edge Detection Pixel Shaders (First Pass)

/**
 * Luma Edge Detection
 *
 * IMPORTANT NOTICE: luma edge detection requires gamma-corrected colors, and
 * thus 'colorTex' should be a non-sRGB texture.
 */

#version 330 core

#define SMAA_RT_METRICS float4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0)
#define SMAA_GLSL_3
#define SMAA_PRESET_HIGH

#define SMAA_INCLUDE_PS 1
#define SMAA_INCLUDE_VS 0

#include "SMAA.h"

out vec4 fragmentColor;

uniform SMAATexture2D(edgesTex);
uniform SMAATexture2D(areaTex);
uniform SMAATexture2D(searchTex);
  
in vec2 fragmentTextureCoordinates;
in vec2 pixcoord;
in vec4 offset0;
in vec4 offset1;
in vec4 offset2;

uniform sampler2D colorTex;

void main()
{
    vec4 offsets[3];
    offsets[0] = offset0;
    offsets[1] = offset1;
    offsets[2] = offset2;

    fragmentColor = SMAABlendingWeightCalculationPS(fragmentTextureCoordinates, pixcoord, offsets, edgesTex, areaTex, searchTex, vec4(0.0));
}