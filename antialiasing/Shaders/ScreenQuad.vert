#version 330 core
layout (location = 0) in vec2 ndcPosition;
layout (location = 1) in vec2 textureCoordinate;

out vec2 fragmentTextureCoordinates;

void main()
{
    gl_Position = vec4(ndcPosition.x, ndcPosition.y, 0.0, 1.0); 
    fragmentTextureCoordinates = textureCoordinate;
}  