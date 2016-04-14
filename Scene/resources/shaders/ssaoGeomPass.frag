#version 440

layout (location = 0) in vec3 view_pos;

// Outgoing position of shader
layout (location = 0) out vec3 position_out;

void main()
{
    // set outgoing
	position_out = view_pos;
}