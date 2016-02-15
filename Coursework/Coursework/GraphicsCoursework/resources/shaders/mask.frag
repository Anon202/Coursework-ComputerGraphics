#version 410

// Captured render
uniform sampler2D tex;
// Alpha map
uniform sampler2D alpha_map;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
    // ***************
    // Sample textures
    // ***************
    

    // ********************************************
    // Final colour is product of these two colours
    // ********************************************
    

    // *****************
    // Ensure alpha is 1
    // *****************
    
}