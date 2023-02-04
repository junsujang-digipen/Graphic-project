
uniform vec3 cameraPos = vec3(0.,130.,150.);
uniform float cameraNear = 0.1;
uniform float cameraFar = 1000.;

in vec2 UV;
in vec3 v_normal;
in vec3 v_pos;
in vec3 a_pos;
in vec3 model_normal;

layout (location=0) out vec3 gPos;
layout (location=1) out vec3 gNormal;
layout (location=2) out vec3 gDiffuse;
layout (location=3) out vec4 gSpecular;
layout (location=4) out vec3 gAmbient;
layout (location=5) out vec3 gEmissive;
layout (location=6) out vec3 gDepthValue;

uniform vec3 MatAmbient = vec3(0.5);
uniform sampler2D MatDiffuse;
uniform sampler2D MatSpecular;
float MatShininess = 10.;
uniform vec3 MatEmissive = vec3(0.);

//tex
uniform vec3 boundMax = vec3(1.);
uniform vec3 boundMin = vec3(-1.);
uniform int texMappingType = 0;
uniform int isGPUMapping = 1;
uniform int isPositionEntity = 1;


void main()
{
	//texture(MatDiffuse, TexCoord).rgb
	//texture(MatSpecular, TexCoord).rgb
	vec2 TexCoord = vec2(0.);
	vec3 Entity = a_pos;
	if(isPositionEntity == 0){
		Entity = normalize(model_normal);
	}
	Material material;
	vec3 mdiffuse = vec3(0.5);
	vec3 mspecular = vec3(0.5);
	const float maxColVal = 255.;
	if(isGPUMapping == 1){
		if(texMappingType == 0){
			TexCoord = calcSphereTexCoord(Entity,boundMin,boundMax);
			mdiffuse = texture(MatDiffuse, TexCoord).rgb;
			mspecular = texture(MatSpecular, TexCoord).rgb;
		}
		else if(texMappingType == 1){
			TexCoord = calcCylindricalTexCoord(Entity,boundMin,boundMax);
			mdiffuse = texture(MatDiffuse, TexCoord).rgb;
			mspecular = texture(MatSpecular, TexCoord).rgb;
		}
		else if(texMappingType == 2){
			TexCoord = calcPlanarTexCoord(Entity,boundMin,boundMax);
			mdiffuse = texture(MatDiffuse, TexCoord).rgb;
			mspecular = texture(MatSpecular, TexCoord).rgb;
		}
		else if(texMappingType == 3){
			TexCoord = calcCubeMapTexCoord(Entity,boundMin,boundMax);
			mdiffuse = texture(MatDiffuse, TexCoord).rgb;
			mspecular = texture(MatSpecular, TexCoord).rgb;
		}
		else {
		}
	}
	else{
		TexCoord = UV;
		if(texMappingType < 4){
			mdiffuse = texture(MatDiffuse, TexCoord).rgb;
			mspecular = texture(MatSpecular, TexCoord).rgb;
		}
	}
	MatShininess = mspecular.r*mspecular.r*maxColVal;
	material = Material(MatAmbient,mdiffuse,mspecular,MatShininess,MatEmissive);
	vec3  n  = normalize(v_normal);

	gPos = v_pos;
	gNormal = n;
	gDiffuse = material.diffuse;
	gSpecular = vec4(material.specular,material.shininess);
	gAmbient = material.ambient;
	gEmissive = material.emissive;
	gDepthValue = vec3(length(v_pos - cameraPos));

}