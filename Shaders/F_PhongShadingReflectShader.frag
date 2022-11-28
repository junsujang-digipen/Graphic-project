
uniform vec3 cameraPos = vec3(0.,0.,150.);
uniform float cameraNear = 0.1;
uniform float cameraFar = 1000.;
uniform vec3 fogCol = vec3(0.,0.,0.);
uniform vec3 GAmbient= vec3(0.,0.,0.);

in vec2 UV;
in vec3 v_normal;
in vec3 v_pos;
in vec3 a_pos;
in vec3 model_normal;
out vec4 col;

//lights
#define MAX_LIGHT_NUMBERS 16 
uniform Light lightSources[MAX_LIGHT_NUMBERS];
uniform int numCurLights = 1;
//

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


//reflect refract
uniform int isReflect = 1;
uniform int isRefract = 1;
uniform float n1 = 1.;
uniform float n2 = 1.33;
uniform float environmentInterpolateValue = 0.95;
uniform float fresnelConst = 1.;

#define CUBE_MAP_NUM 6
#define LEFT 0
#define RIGHT 1
#define BOTTOM 2
#define TOP 3
#define BACK 4
#define FRONT 5
uniform sampler2D EnvironmentTexture[CUBE_MAP_NUM];

void main()
{
	
	vec2 TexCoord = vec2(0.);
	vec3 Entity = a_pos;
	if(isPositionEntity == 0){
		Entity = normalize(model_normal);
	}
	Material material;
	vec3 mdiffuse = vec3(0.5);
	vec3 mspecular = vec3(0.5);
	const float maxColVal = 32.;
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
	vec3 viewDir = normalize(cameraPos - v_pos);
	vec3 color = vec3(0);

	for(int i = 0;i<numCurLights;++i){
		vec3 l = vec3(1.);
		if(lightSources[i].type == 0){
			l = normalize(-lightSources[i].direction);
		}else if(lightSources[i].type == 1){
			l = normalize(lightSources[i].position - v_pos);
		}else if(lightSources[i].type == 2){
			l = normalize(lightSources[i].position - v_pos);
		}
		
		float nl = max(dot(n, l), 0.);

		//vec3 reflectDir = computeReflect(l, n); 
		vec3 reflectDir = reflect(n, l);  \
		float sf;
		if(max(dot(viewDir, reflectDir), 0.0) > 0.0){
			 sf = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		}else{
			 sf = 0.0;
		}
		vec3 ambient =  lightSources[i].ambient * material.ambient;
		vec3 diffuse = lightSources[i].diffuse * material.diffuse * nl;
		vec3 specular = lightSources[i].specular * material.specular * sf;	

		//compute Att
		float distLight = length(lightSources[i].position -v_pos);
		float Att;
		
		if(lightSources[i].type == 0){
			Att = 1.f;
		}else{
			Att = min(1.0f/(lightSources[i].constant + lightSources[i].linear*distLight + lightSources[i].quadratic*distLight*distLight),1.0);
		}

		//compute spot light
		if(lightSources[i].type == 2){
			float theta = (dot(normalize(-l), normalize(lightSources[i].direction))); 
    		float epsilon = (lightSources[i].innerCut - lightSources[i].outerCut);
    		float intensity = clamp((theta - lightSources[i].outerCut) / epsilon, 0.0, 1.0);
			intensity = pow(intensity,lightSources[i].fallOff);
			//ambient *= intensity;
			diffuse  *= intensity;
    		specular *= intensity;
		}

		color += Att*(ambient + diffuse + specular);
	}
	//color /= numCurLights;


	//environment mapping
	int envMappingType = isReflect+isRefract;
	vec3 reflectfract = vec3(0.);
	float kValue = n1/n2;
	const float EtaR = 1.1;
	const float EtaG = 1.2;
	const float EtaB = 1.3;
	if(isReflect==1 && envMappingType!=2){
		vec3 environmentUV = calcSkyBoxCubeMapTexCoordWithSide(computeReflect(viewDir, n),boundMin,boundMax);
		reflectfract = texture(EnvironmentTexture[int(environmentUV.z)], environmentUV.xy).rgb;
	}else if(isRefract==1 && envMappingType!=2){
		vec3 environmentUV = calcSkyBoxCubeMapTexCoordWithSide(computeRefract(viewDir, n, kValue),boundMin,boundMax);
		reflectfract = texture(EnvironmentTexture[int(environmentUV.z)], environmentUV.xy).rgb;
	}else if(envMappingType==2){
		vec3 environmentUVreflect = calcSkyBoxCubeMapTexCoordWithSide(computeReflect(viewDir, n),boundMin,boundMax);
		vec3 environmentUVrefractR = calcSkyBoxCubeMapTexCoordWithSide(computeRefract(viewDir, n, kValue*EtaR),boundMin,boundMax);
		vec3 environmentUVrefractG = calcSkyBoxCubeMapTexCoordWithSide(computeRefract(viewDir, n, kValue*EtaG),boundMin,boundMax);
		vec3 environmentUVrefractB = calcSkyBoxCubeMapTexCoordWithSide(computeRefract(viewDir, n, kValue*EtaB),boundMin,boundMax);
		float f = ((1-kValue)*(1-kValue))/((1+kValue)*(1+kValue));
		float ratio = f + (1-f)*pow(1 - dot(viewDir,n), fresnelConst);
		vec3 temp = vec3(
			texture(EnvironmentTexture[int(environmentUVrefractR.z)], environmentUVrefractR.xy).r,
			texture(EnvironmentTexture[int(environmentUVrefractG.z)], environmentUVrefractG.xy).g,
			texture(EnvironmentTexture[int(environmentUVrefractB.z)], environmentUVrefractB.xy).b
		);
		reflectfract = mix(
		texture(EnvironmentTexture[int(environmentUVreflect.z)], environmentUVreflect.xy).rgb,
		temp,
		ratio);
	}
	color = mix(color,reflectfract,environmentInterpolateValue);

	//fog
	color += material.emissive + GAmbient*material.ambient;
	float fog = (cameraFar - length(cameraPos - v_pos) )/(cameraFar - cameraNear);

	color = color*fog + (1-fog)*fogCol;
	col = vec4(color,1.0);
}