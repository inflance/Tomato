#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec4 aColor;   
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in float aTexIndex;
layout (location = 4) in float aTilingFactor;


out vec4 vColor;
out vec2 vTexcoord; 
out float vTexIndex; 
out float vTilingFactor;

uniform mat4 uViewProjection;

void main()
{
    vColor = aColor;
    vTexcoord = aTexcoord; 
    vTexIndex = aTexIndex;
    vTilingFactor = aTilingFactor;
    gl_Position = uViewProjection  *  vec4(aPos, 1.0);
  
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec4 vColor;
in vec2 vTexcoord;
in float vTexIndex;
in float vTilingFactor;

uniform sampler2D uTextures[32];


void main()
{
   FragColor = texture(uTextures[int(vTexIndex)], vTexcoord * vTilingFactor) * vColor;
   //FragColor = vColor;
}