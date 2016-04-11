#version 440

// Incoming texture containing frame information
uniform sampler2D tex;

// Our colour filter - calculates colour intensity
const vec3 intensity = vec3(0.299,0.587,0.184);

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
    // *********************
    // Sample texture colour
    // *********************
    vec4 colourSample = texture(tex, tex_coord);

    // ********************
    // Calculate grey value
    // ********************


	float i = dot(colourSample.rgb, intensity);

	colour = vec4(i, i, i, 1.0);
	
    // ********************************
    // Use greyscale to as final colour
    // - ensure alpha is 1
    // ********************************

	colour.a = 1.0f;

    
}