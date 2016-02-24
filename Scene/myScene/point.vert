/*#version 440

// MVP transformation matrix
uniform mat4 MVP;
// M transformation matrix
uniform mat4 M;
// N transformation matrix
uniform mat3 N;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming texture coordinate
layout(location = 10) in vec2 tex_coord;

// Outgoing vertex position
layout(location = 0) out vec3 vertex_position;
// Transformed normal
layout(location = 1) out vec3 transformed_normal;
// Outgoing tex_coord
layout(location = 2) out vec2 vertex_tex_coord;

void main()
{
	// Calculate screen position
	gl_Position = MVP * vec4(position, 1.0);
	// Calculate vertex world position
	vertex_position = (M * vec4(position, 1.0)).xyz;
	// Transform normal
	transformed_normal = N * normal;
	// Pass through tex_coord
	vertex_tex_coord = tex_coord;

}

#version 440

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming texture coordinate
layout(location = 10) in vec2 tex_coord_in;

// Outgoing vertex position
layout(location = 0) out vec3 vertex_position;
// Outgoing transformed normal
layout(location = 1) out vec3 transformed_normal;
// Outgoing texture coordinate
layout(location = 2) out vec2 tex_coord_out;

void main()
{
	// Set position
	gl_Position = MVP * vec4(position, 1);
	// **************************************
	// Output other values to fragment shader
	// **************************************

	vertex_position = vec3(M * vec4(position, 1));

	transformed_normal = N * normal;

	tex_coord_out = tex_coord_in;


}*/