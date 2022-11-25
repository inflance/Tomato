#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec4 aColor;   
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in float aTexIndex;
layout (location = 4) in float aTilingFactor;
layout (location = 5) in int GID;


out vec4 vColor;
out vec2 vTexcoord; 
out float vTexIndex; 
out float vTilingFactor;
out int vGID;

uniform mat4 u_ViewProjection;

void main()
{
    vColor = aColor;
    vTexcoord = aTexcoord; 
    vTexIndex = aTexIndex;
    vTilingFactor = aTilingFactor;
    vGID = GID;
    gl_Position = u_ViewProjection  *  vec4(aPos, 1.0);
    
}

#type fragment
#version 450 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int color1;

in vec4 vColor;
in vec2 vTexcoord;
in float vTexIndex;
in float vTilingFactor;
in flat int vGID;

uniform sampler2D uTextures[32];

void main()
{
   FragColor = texture(uTextures[int(vTexIndex)], vTexcoord * vTilingFactor) * vColor;
   color1 = vGID;
}