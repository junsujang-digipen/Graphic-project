
uniform vec3 cameraPos = vec3(0.,130.,150.);
uniform float cameraNear = 0.1;
uniform float cameraFar = 1000.;
uniform vec3 fogCol = vec3(0.,0.,0.);
uniform vec3 GAmbient= vec3(0.,0.,0.);

in vec3 v_normal;
in vec3 v_pos;
in vec3 a_pos;
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

void main()
{
	//texture(MatDiffuse, TexCoord).rgb
	//texture(MatSpecular, TexCoord).rgb
	vec2 TexCoord;
	Material material;
	vec3 mdiffuse;
	vec3 mspecular;
	const float maxColVal = 255.;
	if(texMappingType == 0){
		TexCoord = calcSphereTexCoord(a_pos,boundMin,boundMax);
		mdiffuse = texture(MatDiffuse, TexCoord).rgb;
		mspecular = texture(MatSpecular, TexCoord).rgb;
	}
	else if(texMappingType == 1){
		TexCoord = calcCylindricalTexCoord(a_pos,boundMin,boundMax);
		mdiffuse = texture(MatDiffuse, TexCoord).rgb;
		mspecular = texture(MatSpecular, TexCoord).rgb;
	}
	else if(texMappingType == 2){
		TexCoord = calcPlanarTexCoord(a_pos,boundMin,boundMax);
		mdiffuse = texture(MatDiffuse, TexCoord).rgb;
		mspecular = texture(MatSpecular, TexCoord).rgb;
	}
	else if(texMappingType == 3){
		TexCoord = calcCubeMapTexCoord(a_pos,boundMin,boundMax);
		mdiffuse = texture(MatDiffuse, TexCoord).rgb;
		mspecular = texture(MatSpecular, TexCoord).rgb;
	}
	else {
		TexCoord = vec2(0.);
		mdiffuse = vec3(0.5);
		mspecular = vec3(0.5);
	}
	MatShininess = mspecular.r*mspecular.r*maxColVal;
	material = Material(MatAmbient,mdiffuse,mspecular,MatShininess,MatEmissive);
	vec3  n  = normalize(v_normal);
	vec3 viewDir = normalize(cameraPos - v_pos);
	vec3 color = vec3(0);

	for(int i = 0;i<numCurLights;++i){
		vec3 l;
		if(lightSources[i].type == 0){
			l = normalize(-lightSources[i].direction);
		}else if(lightSources[i].type == 1){
			l = normalize(lightSources[i].position - v_pos);
		}else if(lightSources[i].type == 2){
			l = normalize(lightSources[i].position - v_pos);
		}
		
		float nl = max(dot(n, l), 0.);

		vec3 reflectDir = computeReflect(l, n);  
		float sf = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		
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
	//fog
	color += material.emissive + GAmbient*material.ambient;
	float fog = (cameraFar - length(cameraPos - v_pos) )/(cameraFar - cameraNear);

	color = color*fog + (1-fog)*fogCol;
	col = vec4(color,1.0);
}