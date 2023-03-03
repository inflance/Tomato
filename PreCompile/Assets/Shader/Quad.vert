#version 450

layout(location = 0) in vec3 inPositions;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inColor;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in float inTexIndex;
layout(location = 5) in float inTilingFactor;


layout(location = 0) out vec4 outColor;
layout(location = 1) out vec3 outWorldPos;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outTexCoord;
layout(location = 4) out float outTexIndex;
layout(location = 5) out float outTilingFactor;


layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPositions,1.0f);
	outColor = inColor;
	outTexCoord = inTexCoord;
	outTexIndex = inTexIndex;
	outWorldPos = vec3(ubo.model *  vec4(inPositions,1.0f));
	outNormal = vec3(ubo.model * vec4(inNormal, 1.0f));
	outTilingFactor = inTilingFactor;
}