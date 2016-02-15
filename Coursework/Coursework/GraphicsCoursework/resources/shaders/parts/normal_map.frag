vec3 calc_normal(in vec3 normal, in vec3 tangent, in vec3 binormal, in sampler2D normal_map, in vec2 tex_coord)
{
	// ****************************************************************
	// Ensure normal, tangent and binormal are unit length (normalized)
	// ****************************************************************
	
	// *****************************
	// Sample normal from normal map
	// *****************************
	
	// ************************************
	// Transform components to range [0, 1]
	// ************************************
	
	// *******************
	// Generate TBN matrix
	// *******************
	
	// ****************************************
	// Return sampled normal transformed by TBN
	// - remember to normalize
	// ****************************************
	return vec3(1.0, 0.0, 0.0); // Change!!!
}