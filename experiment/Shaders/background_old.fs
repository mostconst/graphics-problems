#version 110
uniform vec4 topcolor;
uniform vec4 botcolor;
varying vec2 v_uv;
void main()
{
   gl_FragColor = botcolor * (1.0 - v_uv.y) + topcolor * v_uv.y;
}