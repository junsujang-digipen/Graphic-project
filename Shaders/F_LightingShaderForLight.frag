in vec3 v_normal;
in vec3 v_pos;
out vec4 col;

uniform vec3 OBJColor;

void main()
{

	col = vec4(OBJColor,1.0 + v_normal.x*0.00001);
}