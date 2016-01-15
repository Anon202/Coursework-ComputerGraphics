// Directional light structure
#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light
{
	vec4 ambient_intensity;
	vec4 light_colour;
	vec3 light_dir;
};
#endif

// A material structure
#ifndef MATERIAL
#define MATERIAL
struct material
{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};
#endif

// Calculates the directional light
vec4 calculate_direction(in directional_light light, in material mat, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
	vec4 colour = vec4(0.0, 0.0, 0.0, 1.0);
	// ***************************
	// Calculate ambient component
	// ***************************

	// ***************************
	// Calculate diffuse component
	// ***************************

	// *********************
	// Calculate half vector
	// *********************

	// ****************************
	// Calculate specular component
	// ****************************

	// **************************
	// Calculate colour to return
	// - remember alpha = 1
	// **************************

	// *************
	// Return colour
	// *************
	return colour;
}