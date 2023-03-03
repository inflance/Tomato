#version 450

layout(location = 0) in vec4 inPositions;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in float inTexIndex;
layout(location = 4) in float inTilingFactor;


layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out float outTexIndex;
layout(location = 3) out float outTilingFactor;
layout(location = 4) out float outLOD;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	float lod;
} ubo;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPositions);
	outColor = inColor;
	outTexCoord = inTexCoord;
	outTexIndex = inTexIndex;
	outTilingFactor = inTilingFactor;
	outLOD = ubo.lod;
}