#version 450
layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in float inTexIndex;
layout(location = 3) in float inTilingFactor;
layout(location = 4) in float inLOD;


layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;

void main() {
    outColor = inColor * vec4(texture(texSampler, inTexCoord * inTilingFactor, inLOD));
}