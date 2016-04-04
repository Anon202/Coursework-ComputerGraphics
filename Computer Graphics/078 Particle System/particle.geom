#version 440

// Time passed since last frame
uniform float delta_time;

// Incoming geometry
layout (points) in;
// Outgoing geometry
layout (points, max_vertices = 1) out;

// Incoming position data
layout (location = 0) in vec3 position[];
// Incoming velocity data
layout (location = 1) in vec3 velocity[];

// Outgoing position after update
layout (location = 0) out vec3 position_out;
// Outgoing velocity
layout (location = 1) out vec3 velocity_out;

void main()
{
    // ************************************************
    // Update the position using standard velocity step
    // ************************************************
    position_out = position[0] + (velocity[0] * delta_time);

    // *****************************************
    // Ensure particle does not go out of bounds
    // - if y > 5 set y to 0
    // *****************************************
    if (position_out.y > 5)
		position_out.y = 0;

	// ***********
    // Output data
	// ***********
	velocity_out = velocity[0];
    

    // Emit vertex and end primitive
    EmitVertex();
    EndPrimitive();
}