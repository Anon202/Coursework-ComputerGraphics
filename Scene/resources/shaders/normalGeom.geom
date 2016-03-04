#version 440

uniform mat4 VP;

layout (points) in;

layout (line_strip, max_vertices = 2) out;
layout (location = 1) in vec4 normal[];

//layout (location = 0) out
layout (location = 0) out vec4 vertex_colour;

void main()
{

	vertex_colour = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = VP * gl_in[0].gl_Position;
	EmitVertex();

	vertex_colour = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 endPoint = gl_in[0].gl_Position + normal[0];
	endPoint.w = 1.0f;

	gl_Position = VP * endPoint;
	EmitVertex();

	EndPrimitive();
	
}