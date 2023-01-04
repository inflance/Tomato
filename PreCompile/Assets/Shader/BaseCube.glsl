// Basic Texture Shader
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

layout (location = 0) out vec3 v_Normal;
layout (location = 1) out flat int v_EntityID;
layout (location = 2) out vec3 v_FragPos;
layout (location = 3) out vec2 v_TexCoord;

void main()
{
	v_EntityID = a_EntityID;
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct Material {
    vec3 Ambient;
    sampler2D Diffuse;
    sampler2D Specular;
    float Shininess;
};

struct DirectionLight{
    vec3 Color;
    vec3 Direction;
    float Intensity;
};

struct PointLight {
    vec3 Color;
    vec3 Position;
    float Constant;
    float Linear;
    float Quadratic;
    float Intensity;
};

layout (location = 0) in vec3 v_Normal;
layout (location = 1) in flat int v_EntityID;
layout (location = 2) in vec3 v_FragPos;
layout (location = 3) in vec2 v_TexCoord;

uniform Material u_Material;
uniform DirectionLight u_DirLight;
uniform PointLight u_PointLights[50];
uniform int u_DirLightSize;
uniform int u_PointLightSize;
uniform vec4 u_Color;
uniform vec3 u_CameraPos;

vec3 CalcDirectionLight(DirectionLight dirLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLight.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);

    vec3 ambient  = u_Material.Ambient * texture(u_Material.Diffuse, v_TexCoord).rgb;
    vec3 diffuse  = diff * texture(u_Material.Diffuse, v_TexCoord).rgb;
    vec3 specular = spec * texture(u_Material.Specular, v_TexCoord).rgb;
    return vec3(ambient + diffuse + specular) * dirLight.Color * dirLight.Intensity ;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
    // 衰减
    float dis    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * dis + 
                 light.Quadratic * (dis * dis));    
    // 合并结果
    vec3 ambient  = u_Material.Ambient * texture(u_Material.Diffuse, v_TexCoord).rgb;
    vec3 diffuse  = diff * texture(u_Material.Diffuse, v_TexCoord).rgb;
    vec3 specular = spec * texture(u_Material.Specular, v_TexCoord).rgb;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular)* light.Color * light.Intensity;
}

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_CameraPos - v_FragPos);

    vec3 result = {0.0f, 0.0f, 0.0f}; 
    for(int i = 0; i < u_DirLightSize; i++)
        result = CalcDirectionLight(u_DirLight, norm, viewDir);

    for(int i = 0; i < u_PointLightSize; i++)
        result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

	o_Color = u_Color * vec4(result, 1.0f);
	//o_Color = texture(u_Diffuse, v_TexCoord);
	o_EntityID = v_EntityID;
}
