#version 440

// Incoming texture containing frame information
uniform sampler2D tex;

// screen res for creating circle
uniform vec2 resolution;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
    // Sample texture colour
    vec4 colourSample = texture(tex, tex_coord);

	// calculate the position vector for current pixel
	vec2 position = (gl_FragCoord.xy / resolution) - vec2(0.5);

	// invert length
	float len = length(position);
	len = 1.0 - len;

	colourSample *= len; // multiply output by inverted length
	
	colour = vec4(colourSample.rgb, 1.0);

}