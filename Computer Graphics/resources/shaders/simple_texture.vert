#version 440

// Model view projection matrix
uniform mat4 MVP;


uniform float time;

layout (location = 0) in vec3 position;
layout (location = 10) in vec2 tex_coord_in;

layout (location = 0) out vec2 tex_coord_out;
layout (location = 1) out vec2 uv;

void main()
{
	// Calculate screen position of vertex
	gl_Position = MVP * vec4(position, 1.0);
	// Output texture coordinate to fragement shader
	tex_coord_out = tex_coord_in;

	uv = vec2(tex_coord_in.x, tex_coord_in.y + (time * 0.5));
}