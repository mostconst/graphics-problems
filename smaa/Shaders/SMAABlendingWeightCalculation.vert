#version 330 core
layout (location = 0) in vec2 ndcPosition;
layout (location = 1) in vec2 textureCoordinate;

out vec2 fragmentTextureCoordinates;
out vec4 offset[3];
out vec2 pixcoord;

void main()
{
    gl_Position = vec4(ndcPosition.x, ndcPosition.y, 0.0, 1.0); 
    fragmentTextureCoordinates = textureCoordinate;

    pixcoord = textureCoordinate * vec4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0).zw;

    // We will use these offsets for the searches later on (see @PSEUDO_GATHER4):
    offset[0] = (vec4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0).xyxy * vec4(-0.25, -0.125, 1.25, -0.125) + textureCoordinate.xyxy);
    offset[1] = (vec4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0).xyxy * vec4(-0.125, -0.25, -0.125, 1.25) + textureCoordinate.xyxy);

    // And these for the searches, they indicate the ends of the loops:
    offset[2] = (vec4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0).xxyy * vec4(-2.0, 2.0, -2.0, 2.0) * float(16) + vec4(offset[0].xz, offset[1].yw));
}