#version 440

// Incoming position
layout (location = 5) in vec3 position;
// Incoming velocity
layout (location = 6) in vec3 velocity;

//incoming lifetime
layout (location = 7) in vec2 lifetime;

// Outgoing position
layout (location = 0) out vec3 position_out;
// Outgoing velocity
layout (location = 1) out vec3 velocity_out;
// Outgoing life
layout (location = 2) out vec2 lifetime_out;

void main()
{
    // Pass through the values
    position_out = position;
    velocity_out = velocity;
	lifetime_out = lifetime;
}