#version 440

// MV transformation matrix
uniform mat4 MV;

// Incoming position
layout (location = 5) in vec3 position;

// Outgoing height
layout (location = 0) out float height;

void main()
{
	// Transform position into camera space
    gl_Position = MV * vec4(position, 1.0);
	// Output original height
    height = position.y;
}