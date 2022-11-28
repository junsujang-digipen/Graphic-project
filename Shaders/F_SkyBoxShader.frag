

#define CUBE_MAP_NUM 6
#define LEFT 0
#define RIGHT 1
#define BOTTOM 2
#define TOP 3
#define BACK 4
#define FRONT 5
uniform sampler2D BGTexture[CUBE_MAP_NUM];
uniform vec3 boundMax = vec3(1.);
uniform vec3 boundMin = vec3(-1.);
in vec3 UV;
in vec3 a_pos;

out vec4 col;

void main()
{	
	vec3 Entity = a_pos;
	vec3 TexCoord = calcSkyBoxCubeMapTexCoordWithSide(Entity,boundMin,boundMax);
	vec3 color = texture(BGTexture[int(TexCoord.z)], TexCoord.xy).rgb;
	col = vec4(color,1.0);
}