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

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out flat int v_EntityID;

void main()
{
	v_TexCoord = a_TexCoord;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	//gl_Position = vec4(0.0, -1.0, 0.0, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Diffuse;

void main()
{
	o_Color = texture(u_Diffuse, v_TexCoord);
	o_EntityID = v_EntityID;
}
