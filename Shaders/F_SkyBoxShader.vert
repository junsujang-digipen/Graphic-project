
layout(location = 0) in vec3 modelPosition;

layout(location = 3) in vec2 uv;
out vec3 UV;
out vec3 a_pos;

uniform mat4 modelToWorldMat;
uniform mat4 WorldToCameraMat;

void main()
{
	a_pos = modelPosition;
	UV = vec3(uv,0.);
	//vec3 Entity = modelPosition;
	//vec3 boundMax = vec3(1.);
	//vec3 boundMin = vec3(-1.);
	//UV = calcSkyBoxCubeMapTexCoordWithSide(Entity,boundMin,boundMax);
	
	gl_Position.xyzw = WorldToCameraMat*modelToWorldMat*vec4(modelPosition,1);
}