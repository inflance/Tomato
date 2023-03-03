#version 450

#define DirLight  0;
#define PointLight  1;
#define SpotLight  2;
#define MAX_LIGHTS 50;

struct Light
{
	int light_type;
		
	vec3 color;
	//z axis
	vec3 direction;
	float intensity;
	float constant;
	float linear;
	float quadratic;
};

