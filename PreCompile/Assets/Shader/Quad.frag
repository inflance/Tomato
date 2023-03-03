#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

const uint DirLight = 0;
const uint PointLight =  1;
const uint SpotLight = 2;

const int MAX_LIGHTS = 50;
const int MAX_MATERIAL = 50;

struct Light
{
	uint light_type;
		
	vec3 color;
	//z axis
    vec3 position;
	vec3 direction;
	float intensity;
	float constant;
	float linear;
	float quadratic;
    float cut_off;
    float outer_cut_off;
};
layout(binding = 1) uniform UniformBufferObject 
{
   Light lights[MAX_LIGHTS];
   Material materials[MAX_MATERIAL];
   vec3 camera_pos;
   uint light_count;
} ubo;

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec3 inWorldPos;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in float inTexIndex;
layout(location = 5) in float inTilingFactor;


layout(location = 0) out vec4 outColor;
layout(binding = 2) uniform sampler2D texSampler;

vec3 Lighting(Material material)
{
    vec3 normal = normalize(inNormal);
    vec3 res = vec3(0, 0, 0);
	if(ubo.light_count == 0)
	{
		return inColor.rgb;
	}
	
    vec3 viewDir = normalize(ubo.camera_pos - inWorldPos);
    for(int i = 0; i < ubo.light_count; i++)
    {
        Light light = ubo.lights[i];
        vec3 color;
		vec3 lightColor = light.color.xyz * light.intensity;
		vec3 indirect = vec3(0,0,0);
		vec3 lightDir = vec3(0,0,0);
        float value = 1.0;
        float att = 1.0;
        if(light.light_type == DirLight)
        {

            lightDir = normalize(-light.direction);
            indirect = reflect(-lightDir, normal);
          
        }else if(light.light_type == PointLight)
        {
            lightDir = normalize(light.position - inWorldPos);
            indirect = reflect(-lightDir, normal);
            float dis    = length(light.position - inWorldPos);
            float attenuation = 1.0 / (light.constant + light.linear * dis + 
                 light.quadratic * (dis * dis));
            att = attenuation;
        }else if (light.light_type == SpotLight){
            lightDir = normalize(-light.direction);
            indirect = reflect(-lightDir, normal);
            float theta = dot(lightDir, normalize(-light.direction)); 
            float epsilon = (light.cut_off - light.outer_cut_off);
            float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
            value = intensity;

            float dis    = length(light.position - inWorldPos);
            float attenuation = 1.0 / (light.constant + light.linear * dis + 
                 light.quadratic * (dis * dis));
            att = attenuation;
        }
        float diff = max(dot(inNormal, lightDir), 0.0);
        float spec = pow(max(dot(viewDir, indirect), 0.0), material.shininess);

        vec3 ambient  = material.ambient * lightColor  * att;
        vec3 diffuse  = diff * material.diffuse * lightColor * value * att;
        vec3 specular = spec * material.specular * lightColor * value * att ;
        color = vec3(ambient + diffuse + specular) * inColor.rgb;
        res += color;
    }
  return res;
}

void main() {
    Material material = ubo.materials[int(inTexIndex)];
    vec3 color = Lighting(material); 
    outColor = vec4(color, inColor.a) * vec4(texture(texSampler, inTexCoord * inTilingFactor));
}