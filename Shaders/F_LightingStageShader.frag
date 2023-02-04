
uniform vec3 cameraPos = vec3(0.,130.,150.);
uniform float cameraNear = 0.1;
uniform float cameraFar = 1000.;
uniform vec3 fogCol = vec3(0.,0.,0.);
uniform vec3 GAmbient= vec3(0.,0.,0.);

in vec2 UV;
out vec4 col;

//lights
#define MAX_LIGHT_NUMBERS 16 
uniform Light lightSources[MAX_LIGHT_NUMBERS];
uniform int numCurLights = 1;
//

layout (binding=0) uniform sampler2D gPos;
layout (binding=1) uniform sampler2D gNormal;
layout (binding=2) uniform sampler2D gDiffuse;
layout (binding=3) uniform sampler2D gSpecular;
layout (binding=4) uniform sampler2D gAmbient;
layout (binding=5) uniform sampler2D gEmissive;
layout (binding=6) uniform sampler2D depthBuffer;

void main()
{
	Material material;
	const float maxColVal = 255.;
	material.ambient = texture(gAmbient,UV).rgb;
	material.diffuse= texture(gDiffuse,UV).rgb;
	material.specular= texture(gSpecular,UV).rgb;
	material.shininess= material.specular.r*material.specular.r*maxColVal;
	material.emissive= texture(gEmissive,UV).rgb;

	vec3  n  = texture(gNormal,UV).rgb;
	vec3 v_pos = texture(gPos,UV).rgb;
	
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
	//fog
	color += material.emissive + GAmbient*material.ambient;
	float fog = (cameraFar - length(cameraPos - v_pos) )/(cameraFar - cameraNear);

	color = color*fog + (1-fog)*fogCol;
	col = vec4(color,1.0);
}