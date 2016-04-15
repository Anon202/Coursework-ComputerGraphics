#version 440

// Model view projection matrix
uniform mat4 MVP;

// Incoming value for the position
layout (location = 5) in vec3 position;

layout (location = 1) in vec4 in_colour;

layout (location = 0) out vec4 out_colour;

// Main vertex shader function
void main()
{
	// Calculate screen position of vertex
	gl_Position = MVP * vec4(position, 1.0);
	
	out_colour = in_colour;
}