#version 410

// Captured render
uniform sampler2D tex;
// Alpha map
uniform sampler2D alpha_map;


// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;
layout (location = 1) in vec2 uv;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
    // ***************
    // Sample textures
    // ***************



    vec4 texSam = texture(tex, tex_coord);
	vec4 alpSam = texture(alpha_map, uv);

    // ********************************************
    // Final colour is product of these two colours
    // ********************************************
	
    colour = texSam * alpSam;

    // *****************
    // Ensure alpha is 1
    // *****************
    colour.a = 1;

	
	/*vec4 blendCol = texture(alpha_map, tex_coord);

	// *******************************************************
	// Mix the main samples using r component from blend value
	// *******************************************************
	vec4 bac = vec4(0.0, 0.0, 0.0, 1.0);
	out_colour = mix(bac, colour, blendCol.r);
		
		*/
}