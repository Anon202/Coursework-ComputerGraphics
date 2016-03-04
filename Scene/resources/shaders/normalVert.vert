#version 440

// Model view projection matrix
uniform mat4 VP;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 0) out vec3 vertex_position;
layout (location = 1) out vec4 out_norm;


void main()
{
	// Calculate screen position of vertex
	gl_Position = vec4(position, 1.0);

	vertex_position = position;
	out_norm = vec4(normal, 1.0);

}