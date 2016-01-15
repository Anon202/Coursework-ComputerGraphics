#version 440

// Outgoing pixel colour
layout (location = 0) out vec4 out_colour;

void main()
{
	// Simply set outgoing colour
	out_colour = vec4(1.0, 0.0, 0.0, 1.0);
}