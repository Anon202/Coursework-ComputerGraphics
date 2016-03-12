#version 440

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;

uniform float myTime;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 2) in vec3 normal;
// Incoming texture coordinates
layout (location = 10) in vec2 tex_coord_in;

// Outgoing position
layout (location = 0) out vec3 vertex_position;
// Outgoing normal
layout (location = 1) out vec3 transformed_normal;
// Outgoing texture coordinate
layout (location = 2) out vec2 tex_coord_out;

void main()
{
	vec3 position2 = position;

	float y = sin(position2.x * 10.01f + myTime) * 0.4 + cos(position.z * 5.01f + myTime) * 0.2;

	position2.y += y;

	// Set position
	gl_Position = MVP * vec4(position2, 1);

	// **************************************
	// Output other values to fragment shader
	// **************************************
	
	vertex_position = vec3(M * vec4(position2, 1.0f));

	transformed_normal = N * normal;

	vec2 textureScroll = vec2(tex_coord_in.x - myTime * 0.05, tex_coord_in.y); // scroll texture coordinates
	tex_coord_out = textureScroll;

}