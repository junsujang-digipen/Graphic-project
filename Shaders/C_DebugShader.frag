#version 460 core
uniform vec4 u_color = vec4(1.);
out vec4 col;

void main()
{
	col = u_color;
}