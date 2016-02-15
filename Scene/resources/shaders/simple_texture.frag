#version 440

// Sampler used to get texture colour
uniform sampler2D tex;
//uniform sampler2D tex2;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;
// Outgoing colour
layout (location = 0) out vec4 out_colour;

void main()
{
	// ****************************************
	// Set out colour to sampled texture colour
	// ****************************************

	vec4 colour = texture2D(tex, tex_coord);
	
	
	out_colour = colour;
}