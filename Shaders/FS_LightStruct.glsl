
struct Light {
    int type;
    //directional light == 0
    vec3 direction;
    //point light == 1
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    //spot light == 2
    float innerAngle;
    float outerAngle;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
};

