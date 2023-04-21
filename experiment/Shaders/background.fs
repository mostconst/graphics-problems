#version 330 core
uniform vec4 topcolor;
uniform vec4 botcolor;
in vec2 v_uv;
void main()
{
   gl_FragColor = botcolor * (1 - v_uv.y) + topcolor * v_uv.y;
}