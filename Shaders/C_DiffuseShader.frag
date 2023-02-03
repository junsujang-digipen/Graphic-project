#version 460 core

uniform vec3 u_light = vec3(0., 100., 100.);
uniform vec3 u_light_col = vec3(1., 1., 1.);
uniform vec3 u_ambient = vec3(0.5);
uniform vec3 u_materialCol = vec3(1.0, 1.0, 1.0);
uniform vec3 cameraPos = vec3(0.,100.,100.);

in vec3 v_normal;
in vec3 v_pos;

out vec4 col;

void main()
{
	float specularStrength = 0.5;
	float shininess = 10.;
	float ambientStrength = 0.1;

	vec3  n  = normalize(v_normal);
    vec3  l  = normalize(u_light - v_pos);
    float nl = max(dot(n, l), 0.);

	vec3 viewDir = normalize(cameraPos - v_pos);
	vec3 reflectDir = reflect(-l, n);  
	float sf = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	
	vec3 diffuse = nl*u_light_col;
	vec3 specular = specularStrength * sf * u_light_col;	
    vec3 ambient = ambientStrength * u_ambient * u_light_col;

	vec3 color = (ambient + diffuse + specular) * u_materialCol;
	col = vec4(color,1.0);
}