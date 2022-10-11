#version 330 core
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec2 aTexcoord;

out vec2 oTexcoord; 

uniform mat4 m_viewProjection;
uniform mat4 m_transform;

void main()
{
    gl_Position = m_viewProjection * m_transform * vec4(aPos, 1.0);
    oTexcoord = aTexcoord; 
}