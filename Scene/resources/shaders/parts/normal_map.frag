vec3 calc_normal(in vec3 normal, in vec3 tangent, in vec3 binormal, in vec3 sampled_normal, in vec2 tex_coord)
{
	// ****************************************************************
	// Ensure normal, tangent and binormal are unit length (normalized)
	// ****************************************************************

	normal = normalize(normal);
	tangent = normalize(tangent);
	binormal = normalize(binormal);
	
	sampled_normal = (2.0 * sampled_normal) - vec3(1.0, 1.0, 1.0);
	
	// *******************
	// Generate TBN matrix
	// *******************
	mat3 TBN = mat3(tangent.x, tangent.y, tangent.z, binormal.x, binormal.y, binormal.z, normal.x, normal.y, normal.z);
	
	// ****************************************
	// Return sampled normal transformed by TBN
	// - remember to normalize
	// ****************************************
	vec3 trans_normal = TBN * sampled_normal;
	trans_normal = normalize(trans_normal);

	return trans_normal; // Change!!!
}