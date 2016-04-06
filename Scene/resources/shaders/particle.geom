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
// incoming life
layout (location = 2) in vec2 lifetime[];

// Outgoing position after update
layout (location = 0) out vec3 position_out;
// Outgoing velocity
layout (location = 1) out vec3 velocity_out;

layout (location = 3) out vec2 lifetime_out;

void main()
{
    // ************************************************
    // Update the position using standard velocity step
    // ************************************************
	float life = lifetime[0].x + delta_time;
    vec3 newPos = position[0] + (velocity[0] * delta_time);

    // *****************************************
    // Ensure particle does not go out of bounds
    // - if y > 5 set y to 0
    // *****************************************
	if (life > lifetime[0].y) {
		life = 0.0f;
		newPos = vec3(100.0f);
	}

	// ***********
    // Output data
	// ***********
	position_out = newPos;
	velocity_out = velocity[0];
	lifetime_out = vec2(life,lifetime[0].y);
    

    // Emit vertex and end primitive
    EmitVertex();
    EndPrimitive();
}