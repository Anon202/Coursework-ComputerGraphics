#version 440

// Texture to sample
uniform sampler2D tex;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;
// Incoming colour - use for transparency
layout (location = 1) in vec4 colour;

// Outgoing colour
layout (location = 0) out vec4 colour_out;

void main()
{
	// Sample texture and multiply by incoming colour
    colour_out = texture(tex, tex_coord) * colour;
}