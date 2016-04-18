#version 440

// Incoming frame data
uniform sampler2D tex;

// 1.0f / screen width
uniform float inverse_width;
// 1.0f / screen height
uniform float inverse_height;

uniform float radius;
uniform vec2 dir;

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
 //   // **************************
 //   // Start with colour as black
 //   // **************************
	//colour = vec4(0.0, 0.0, 0.0, 1.0);
    

 //   // Loop through each sample vector
 //   for (int i = 0; i < 4; ++i)
 //   {
 //       // *****************************
 //       // Calculate tex coord to sample
 //       // *****************************

	//	vec2 uv = tex_coord + vec2(samples[i].x * inverse_width, samples[i].y * inverse_height);
        

 //       // ******************************************
 //       // Sample the texture and scale appropriately
 //       // - scale factor stored in w component
 //       // ******************************************
	//	colour += texture(tex, uv)*samples[i].w;
 //   }

 //   // *******************
 //   // Ensure alpha is 1.0
 //   // *******************
 //   colour.a = 1.0;

    //this will be our RGBA sum
    vec4 sum = vec4(0.0);

    //our original texcoord for this fragment
    vec2 tc = tex_coord;

    //the amount to blur, i.e. how far off center to sample from 
    //1.0 -> blur by one pixel
    //2.0 -> blur by two pixels, etc.
    float blur = radius*inverse_width*inverse_height; 

    //the direction of our blur
    //(1.0, 0.0) -> x-axis blur
    //(0.0, 1.0) -> y-axis blur
    float hstep = dir.x;
    float vstep = dir.y;

    //apply blurring, using a 9-tap filter with predefined gaussian weights

    sum += texture2D(tex, vec2(tc.x - 4.0*blur*hstep, tc.y - 4.0*blur*vstep)) * 0.0162162162;
    sum += texture2D(tex, vec2(tc.x - 3.0*blur*hstep, tc.y - 3.0*blur*vstep)) * 0.0540540541;
    sum += texture2D(tex, vec2(tc.x - 2.0*blur*hstep, tc.y - 2.0*blur*vstep)) * 0.1216216216;
    sum += texture2D(tex, vec2(tc.x - 1.0*blur*hstep, tc.y - 1.0*blur*vstep)) * 0.1945945946;
					 
    sum += texture2D(tex, vec2(tc.x, tc.y)) * 0.2270270270;
					 
    sum += texture2D(tex, vec2(tc.x + 1.0*blur*hstep, tc.y + 1.0*blur*vstep)) * 0.1945945946;
    sum += texture2D(tex, vec2(tc.x + 2.0*blur*hstep, tc.y + 2.0*blur*vstep)) * 0.1216216216;
    sum += texture2D(tex, vec2(tc.x + 3.0*blur*hstep, tc.y + 3.0*blur*vstep)) * 0.0540540541;
    sum += texture2D(tex, vec2(tc.x + 4.0*blur*hstep, tc.y + 4.0*blur*vstep)) * 0.0162162162;

    //discard alpha for our simple demo, multiply by vertex color and return
   // gl_FragColor = vColor * vec4(sum.rgb, 1.0);

   // colour = texture(tex, tex_coord);

	colour = sum;
}