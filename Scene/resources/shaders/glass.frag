#version 440

uniform sampler2D tex;

layout (location = 0) in vec4 in_primary;	// incoming colours from vertex shader
layout (location = 1) in vec4 in_secondary;
layout (location = 2) in vec2 tex_coord;

layout (location = 0) out vec4 out_colour;	// outgoing fragment/pixel colour

void main()
{
	// colour is per vertex therefore already calculated according to light

	// sample texture, mix with colour and output

	vec4 tex_colour = texture(tex, tex_coord);
	
	out_colour = in_primary * tex_colour + in_secondary;
	out_colour.a = 1.0f;
}