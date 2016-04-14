#version 440

// Model view projection matrix
uniform mat4 MVP;

layout (location = 0) in vec3 position;

layout (location = 0) out vec2 tex_coord_out;


void main()
{
	// Calculate screen position of vertex
	gl_Position = MVP * vec4(position, 1.0);
	// Output texture coordinate to fragement shader
	tex_coord_out = (position.xy + vec2(1.0)) / 2.0;

}