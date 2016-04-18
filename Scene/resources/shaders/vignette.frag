#version 440

// Incoming texture containing frame information
uniform sampler2D tex;

// screen res for creating circle
uniform vec2 resolution;

// Our colour filter - calculates colour intensity
/// can add sepia here
//const vec3 intensity = vec3(0.299,0.587,0.184);

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
    // Sample texture colour
    vec4 colourSample = texture(tex, tex_coord);

	// determine origin
	vec2 position = (gl_FragCoord.xy / resolution) - vec2(0.5);

	float len = length(position);
	len = 1.0 - len;

	//float i = dot(colourSample.rgb, intensity);
	//colour = vec4(i, i, i, 1.0);

	colourSample *= len;
	
	colour = vec4(colourSample.rgb, 1.0);

	//colour.a = 1.0f;
}