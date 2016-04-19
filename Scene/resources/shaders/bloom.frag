#version 440

// Incoming frame data
uniform sampler2D tex;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;


void main()
{
	vec4 colourSample = texture(tex, tex_coord);

	float intensity = colourSample.r + colourSample.g + colourSample.b;
	intensity/=3.0;

	//float brightness = dot(colourSample.rgb, vec3(0.2126, 0.7152, 0.0722));

	if (brightness > 0.9)
		colour = vec4(colourSample.rgb, 1.0);
	else
		colour = vec4(0.0, 0.0, 0.0, 1.0);


}