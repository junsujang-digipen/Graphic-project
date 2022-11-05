
layout(location = 0) in vec3 modelPosition;

layout(location = 1) in vec3 a_normal;
out vec3 v_normal;
out vec3 v_pos;
out vec3 a_pos;

uniform mat4 modelToWorldMat;
uniform mat4 WorldToCameraMat;
uniform mat3 u_normalMatrix;

void main()
{
	a_pos = modelPosition;
	v_normal = u_normalMatrix * a_normal;
	v_pos = vec3(modelToWorldMat*vec4(modelPosition,1));
	gl_Position.xyzw = WorldToCameraMat*modelToWorldMat*vec4(modelPosition,1);
	//gl_Position.w = 1.0;
}