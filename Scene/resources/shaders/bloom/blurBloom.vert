#version 440

// Model view projection matrix
uniform mat4 MVP;

// Incoming value for the position
layout (location = 0) in vec3 position;

layout (location = 1) in vec2 tex_coords;

layout (location = 0) out vec2 tex_coords_out;

void main()
{
	// Calculate screen position of vertex
	gl_Position = MVP * vec4(position, 1.0);
	
	tex_coords_out = tex_coords;
}