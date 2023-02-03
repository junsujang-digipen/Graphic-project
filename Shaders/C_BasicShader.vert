#version 460 core

layout(location = 2) in vec3 modelPosition;

uniform mat4 modelToWorldMat;
uniform mat4 WorldToCameraMat;

void main()
{
	gl_Position.xyzw = WorldToCameraMat*modelToWorldMat*vec4(modelPosition,1);
}