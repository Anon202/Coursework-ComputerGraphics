#version 440

layout (location = 0) in vec2 tex_coord;

// Outgoing colour for the shader
layout (location = 0) out vec4 colour;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

void main()
{             
	vec4 sceneSamp = texture2D(scene, tex_coord);
	vec4 bloomSamp = texture2D(bloomBlur, tex_coord);

	colour = mix(bloomSamp, sceneSamp, bloomSamp.a);
	//colour = mix(sceneSamp, bloomSamp, bloomSamp.r);
}