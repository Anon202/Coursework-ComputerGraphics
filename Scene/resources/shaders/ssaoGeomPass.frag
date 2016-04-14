#version 440

layout (location = 0) in vec3 view_pos;

// Outgoing position of shader
layout (location = 0) out vec4 position_out;

const float NEAR = 2.414f; // Projection matrix's near plane distance
const float FAR = 1000.0f; // Projection matrix's far plane distance
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}

void main()
{
    // set outgoing
	position_out.xyz = view_pos;
	position_out.a = LinearizeDepth(gl_FragCoord.z);
}