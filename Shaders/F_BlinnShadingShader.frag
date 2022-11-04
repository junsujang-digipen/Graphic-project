
uniform vec3 cameraPos = vec3(0.,100.,100.);

in vec3 v_normal;
in vec3 v_pos;

out vec4 col;

//lights
#define MAX_LIGHT_NUMBERS 16 
uniform Light lightSources[MAX_LIGHT_NUMBERS];
uniform int numCurLights = 1;
//

uniform Material material;

void main()
{
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

		vec3 reflectDir = blinnRefl(viewDir, l);  
		float sf = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		
		vec3 ambient =  lightSources[i].ambient * material.ambient;
		vec3 diffuse = lightSources[i].diffuse * material.diffuse * nl;
		vec3 specular = lightSources[i].specular * material.specular * sf;	

		//compute Att
		//compute spot light

		color += lightSources[i].emissive + (ambient + diffuse + specular);
	}
	
	col = vec4(color,1.0);
}