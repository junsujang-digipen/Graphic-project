
layout(location = 0) in vec3 modelPosition;
layout(location = 3) in vec2 uv;

out vec2 UV;

void main()
{
	UV = uv;
	gl_Position.xyzw = vec4(modelPosition,1);
}