#version 440

// Model view projection matrix
uniform mat4 MVP;

// Model View Matrix
uniform mat4 MV;

// Incoming value for the position
layout (location = 0) in vec3 position;

layout (location = 0) out vec3 position_out;

// Main vertex shader function
void main()
{
	// Calculate screen position of vertex
	gl_Position = MVP * vec4(position, 1.0);
	
	// pass view space postion to fragment shader
	position_out = (MV * vec4(position, 1.0)).xyz;
}