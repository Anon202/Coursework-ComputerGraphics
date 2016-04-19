#version 440

// Model view projection matrix
uniform mat4 MVP;

// Incoming value for the position
layout (location = 0) in vec3 position;

layout (location = 1) in vec4 texCoords;

layout (location = 0) out vec4 out_coords;

// Main vertex shader function
void main()
{
	// Calculate screen position of vertex
	gl_Position = vec4(position, 1.0);
	
	out_coords = texCoords;
}

