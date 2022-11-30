#type frag
#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

struct Material{
    //不透明类型
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light{
    vec3 Position;
    vec3 Diffuse;
    vec3 Specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    //环境光因子
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    //镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir  = reflect(-lightDir, norm);//从光源指向片段
    //计算镜面分量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoords = aTexCoords;
}