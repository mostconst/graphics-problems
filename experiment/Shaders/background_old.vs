#version 110
attribute ivec2 vertexId;
varying vec2 v_uv;
void main()
{
   gl_Position = vec4(vertexId.x, vertexId.y, 0.0, 0.5 ) * 4.0 - 1.0;
   v_uv = vec2(gl_Position.xy * 0.5 + 0.5 );
}