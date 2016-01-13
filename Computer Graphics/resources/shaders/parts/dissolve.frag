#version 440

// Determines if a fragment should be discarded based on a dissolve texture and factor
void dissolve(in sampler2D dissolve_texture, in vec2 tex_coord, in float dissolve_factor)
{
	// ********************************************
	// Get dissolve value from the dissolve texture
	// ********************************************
	vec4 dissolve_value = texture(dissolve, tex_coord);
	// ******************************************************
	// If r component is greate than dissolve factor, discard
	// ******************************************************
	if (dissolve_value.r >= dissolve_factor)
		discard;
}