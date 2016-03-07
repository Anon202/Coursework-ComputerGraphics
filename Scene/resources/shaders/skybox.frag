#version 440

// Cubemap texture
uniform samplerCube cubemap;

// Incoming 3D texture coordinate
layout (location = 0) in vec3 tex_coord;

layout (location = 1) in flat int transparency;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
	// ************************
	// Sample texture as normal
	// ************************
	colour = texture(cubemap, tex_coord);
	if (transparency == 0)
	{
		colour.a = 0.5f;
	}
	else
	{	
		colour.a = 1.0f;
	}
	
}