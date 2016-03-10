#version 440

uniform mat4 VP;						// takes in viewProject matrix as centre of object (incoming point is already in worldPos)

layout (points) in;

layout (line_strip, max_vertices = 6) out;		// output line_strip 
layout (location = 1) in float radius[];		// input radius ( for offset of line)

//layout (location = 0) out
layout (location = 0) out vec4 vertex_colour;

void main()
{

	// colour array so each axis has a different coloured line 
	vec4 colours[3] =
	{
		vec4(1.0, 0.0, 0.0, 1.0),
		vec4(0.0, 0.5, 0.5, 1.0),
		vec4(0.0, 0.0, 1.0, 1.0)
	};


	// three axis 
	vec4 axis[3] =
	{
		vec4(1.0, 0.0, 0.0, 1.0),
		vec4(0.0, 1.0, 0.0, 1.0),
		vec4(0.0, 0.0, 1.0, 1.0)
	};


	// pass three times and draw three lines one from each axis
	for (int i = 0; i < 3; ++i)
	{
		vertex_colour = colours[i];					// set colour of vertex
		gl_Position = VP * gl_in[0].gl_Position;	// set position of vertex by transforming into projection space.
		EmitVertex();								// saves to output stream
	
		vertex_colour = colours[i];					

		// 2nd vertex will be position + direction(axis) * length(radius) - set w value to 1
		vec4 endPoint = gl_in[0].gl_Position + axis[i] * radius[0];  
		endPoint.w = 1.0f;
	
		gl_Position = VP * endPoint;	// transform vertex
		EmitVertex();				
	
		EndPrimitive();				// complete primitive on stream (called after two vertices have been emitted as the output primitive is a line)
	
	}
	
}