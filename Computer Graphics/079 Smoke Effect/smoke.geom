#version 440

// P transformation matrix
uniform mat4 P;

// Size of points we are going to output
uniform float point_size;

// Incoming data description
layout (points) in;
// Outgoing data description
layout (triangle_strip, max_vertices = 4) out;

// Incoming vertex height
layout (location = 0) in float height[];

// Outgoing texture coordinate
layout (location = 0) out vec2 tex_coord;
// Outgoing colour - used to blend textures
layout (location = 1) out vec4 colour;

void main()
{
	// Get incoming position
    vec4 position = gl_in[0].gl_Position;
	// Alpha value of colour is .5 - height / 2
    float c = 0.5 - (height[0] / 2.0);

	// *********************
	// Bottom left corner
	// - position
	// - texture coordinate
	// - colour (1, 1, 1, c)
	// *********************
    vec2 va = position.xy + vec2(-0.5, -0.5) * point_size;
    gl_Position = P * vec4(va, position.zw);
    tex_coord = vec2(0.0, 0.0);
    colour = vec4(1.0, 1.0, 1.0, c);
    EmitVertex();

	// *********************
	// Bottom right corner
	// - position
	// - texture coordinate
	// - colour (1, 1, 1, c)
	// *********************
    vec2 vb = position.xy + vec2(0.5, -0.5) * point_size;
    gl_Position = P * vec4(vb, position.zw);
    tex_coord = vec2(1.0, 0.0);
    colour = vec4(1.0, 1.0, 1.0, c);
    EmitVertex();

	// *********************
	// Top left corner
	// - position
	// - texture coordinate
	// - colour (1, 1, 1, c)
	// *********************
    vec2 vd = position.xy + vec2(-0.5, 0.5) * point_size;
    gl_Position = P * vec4(vd, position.zw);
    tex_coord = vec2(0.0, 1.0);
    colour = vec4(1.0, 1.0, 1.0, c);
    EmitVertex();

	// *********************
	// Top right corner
	// - position
	// - texture coordinate
	// - colour (1, 1, 1, c)
	// *********************
    vec2 vc = position.xy + vec2(0.5, 0.5) * point_size;
    gl_Position = P * vec4(vc, position.zw);
    tex_coord = vec2(1.0, 1.0);
    colour = vec4(1.0, 1.0, 1.0, c);
    EmitVertex();

	// End primitive
    EndPrimitive();
}