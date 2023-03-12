#version 450
layout(location = 0) in vec4	inColor;
layout(location = 1) in float	inThickness;
layout(location = 2) in vec2	inLocalPosition;
layout(location = 3) in float	inLOD;


layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;

void main() {
	float fade = 0.01;
	float dist = sqrt(dot(inLocalPosition, inLocalPosition));
	if (dist > 1.0 || dist < 1.0 - inThickness - fade)
	    discard;

	float alpha = 1.0 - smoothstep(1.0f - fade, 1.0f, dist);
	alpha *= smoothstep(1.0 - inThickness - fade, 1.0 - inThickness, dist);
	outColor = inColor ;
	outColor.a = alpha;
}