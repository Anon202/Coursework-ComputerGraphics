#version 440

// Sampler used to get texture colour
uniform sampler2D tex;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;
// Outgoing colour
layout (location = 0) out vec4 out_colour;

//void main()
//{
//	// sample texture
//	vec4 colour = texture2D(tex, tex_coord);

//	// Set out colour to sampled texture colour
//	out_colour = colour;
//}


//uniform float gSampleRad;
uniform mat4 P;

const int MAX_KERNEL_SIZE = 128;
uniform vec3 gKernel[MAX_KERNEL_SIZE];

void main()
{
	// sample the position from the texture
    vec3 Pos = texture(tex, tex_coord).xyz;

    float AO = 0.0;

    for (int i = 0 ; i < MAX_KERNEL_SIZE ; i++) {
        vec3 samplePos = Pos + gKernel[i]; // generate a random point
        vec4 offset = vec4(samplePos, 1.0); // make it a 4-vector
        offset = P * offset; // project on the near clipping plane
        offset.xy /= offset.w; // perform perspective divide
        offset.xy = offset.xy * 0.5 + vec2(0.5); // transform to (0,1) range

        float sampleDepth = texture(tex, offset.xy).b;

        if (abs(Pos.z - sampleDepth) < 0.5) {
            AO += step(sampleDepth,samplePos.z);
        }
    }

    AO = 1.0 - AO/128.0;

    out_colour = vec4(pow(AO, 2.0));
}