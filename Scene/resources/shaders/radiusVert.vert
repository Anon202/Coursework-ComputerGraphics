#version 440

layout (location = 0) in vec3 position;
layout (location = 1) in float radius;

layout (location = 0) out vec3 vertex_position;
layout (location = 1) out float out_radius;


void main()
{
	// pass position through without transforming as transformation is performed in the geom shader
	gl_Position = vec4(position, 1.0);

	vertex_position = position;  // set outgoing to incoming
	out_radius = radius;

}