// Calculates the shadow factor of a vertex
float calculate_shadow(in sampler2D shadow_map, in vec4 light_space_pos)
{
	// *********************************
    // Get light screen space coordinate
	// *********************************
    

	vec2 shadow_tex_coords;
	// ********************************************************
    // Use this to calculate texture coordinates for shadow map
	// ********************************************************
    // This is a bias calculation to convert to texture space
    

	// Check shadow coord is in range
	if (shadow_tex_coords.x < 0 || shadow_tex_coords.x > 1 || shadow_tex_coords.y < 0 || shadow_tex_coords.y > 1)
		return 1.0;
	// ***************************
    // Interested in depths 0 to 1
	// ***************************
    
	// *************************
    // Now sample the shadow map
	// *************************
    
	// ***************************************************************
    // Check if depth is in range.  Add a slight epsilon for precision
	// ***************************************************************
    return 1.0; // Remove!!!
}