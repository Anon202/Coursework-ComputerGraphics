#version 440

// Incoming frame data
uniform sampler2D tex;

// 1.0f / screen width
uniform float inverse_width;
// 1.0f / screen height
uniform float inverse_height;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

//Surrounding pixels to sample and their scale
const vec4 samples[4] = vec4[4]
(
    vec4(-1.0, 0.0, 0.0, 0.25),
    vec4(1.0, 0.0, 0.0, 0.25),
    vec4(0.0, -1.0, 0.0, 0.25),
    vec4(0.0, -1.0, 0.0, 0.25)
);

void main()
{
    // **************************
    // Start with colour as black
    // **************************
	colour = vec4(0.0, 0.0, 0.0, 1.0);
    

    // Loop through each sample vector
    for (int i = 0; i < 4; ++i)
    {
        // *****************************
        // Calculate tex coord to sample
        // *****************************

		vec2 uv = tex_coord + vec2(samples[i].x * inverse_width, samples[i].y * inverse_height);
        

        // ******************************************
        // Sample the texture and scale appropriately
        // - scale factor stored in w component
        // ******************************************
		colour += texture(tex, uv)*samples[i].w;
    }

    // *******************
    // Ensure alpha is 1.0
    // *******************
    colour.a = 1.0;
}