#version 450

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in float inThickness;
layout(location = 2) in vec4 inColor;
layout(location = 3) in vec2 inLocalPosition;


layout(location = 0) out vec4 outColor;
layout(location = 1) out float outThickness;
layout(location = 2) out vec2 outLocalPosition;
layout(location = 3) out float outLOD;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	float lod;
} ubo;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inWorldPosition, 1.0f);
	outColor = inColor;
    outThickness = inThickness;
    outLocalPosition = inLocalPosition;
	outLOD = ubo.lod;
}