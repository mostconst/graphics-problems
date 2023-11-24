#version 330 core
layout (location = 0) in vec2 ndcPosition;
layout (location = 1) in vec2 textureCoordinate;

out vec2 fragmentTextureCoordinates;
out vec4 offset[3];

void main()
{
    gl_Position = vec4(ndcPosition.x, ndcPosition.y, 0.0, 1.0); 
    fragmentTextureCoordinates = textureCoordinate;
    offset[0] = (vec4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0).xyxy * vec4(-1.0, 0.0, 0.0, -1.0) + textureCoordinate.xyxy);
    offset[1] = (vec4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0).xyxy * vec4( 1.0, 0.0, 0.0, 1.0) + textureCoordinate.xyxy);
    offset[2] = (vec4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0).xyxy * vec4(-2.0, 0.0, 0.0, -2.0) + textureCoordinate.xyxy);
}