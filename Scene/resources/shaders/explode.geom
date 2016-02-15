#version 440

// Scale for normal
uniform float explode_factor;

// Layout of incoming data
layout (triangles) in;
// Layout of outgoing data
layout (triangle_strip, max_vertices = 3) out;

void main()
{
	// *********************
	// Calculate Face Normal
	// *********************
	

	for(int i = 0; i < 3; i++)
	{
		// ************************
		// Change triangle position
		// ************************
		

		// ***********
		// Emit Vertex
		// ***********
		
	}
	// *************
	// End Primitive
	// *************
	
}