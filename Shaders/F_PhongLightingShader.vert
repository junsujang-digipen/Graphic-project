
layout(location = 0) in vec3 modelPosition;

layout(location = 1) in vec3 a_normal;

uniform mat4 modelToWorldMat;
uniform mat4 WorldToCameraMat;
uniform mat3 u_normalMatrix;

uniform vec3 u_light = vec3(0., 100., 100.);
uniform vec3 u_light_col = vec3(1., 1., 1.);
uniform vec3 u_ambient = vec3(0.5);
uniform vec3 u_materialCol = vec3(1.0, 1.0, 1.0);
uniform vec3 cameraPos = vec3(0.,100.,100.);

out vec3 out_color;


void main()
{
	vec3 v_normal = u_normalMatrix * a_normal;
	vec3 v_pos = vec3(modelToWorldMat*vec4(modelPosition,1));
	gl_Position.xyzw = WorldToCameraMat*modelToWorldMat*vec4(modelPosition,1);

	float specularStrength = 0.5;
	float shininess = 10.;
	float ambientStrength = 0.1;

	vec3  n  = normalize(v_normal);
    vec3  l  = normalize(u_light - v_pos);
    float nl = max(dot(n, l), 0.);

	vec3 viewDir = normalize(cameraPos - v_pos);
	vec3 reflectDir = computeReflect(l, n);  
	float sf = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	
	vec3 diffuse = nl*u_light_col;
	vec3 specular = specularStrength * sf * u_light_col;	
    vec3 ambient = ambientStrength * u_ambient * u_light_col;

	vec3 color = (ambient + diffuse + specular) * u_materialCol;
	out_color = color;
}