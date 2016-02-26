// Spot light data
#ifndef SPOT_LIGHT
#define SPOT_LIGHT
struct spot_light
{
	vec4 light_colour;
	vec3 position;
	vec3 direction;
	float constant;
	float linear;
	float quadratic;
	float power;
};
#endif

// Material data
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

// Spot light calculation
vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
	vec4 colour = vec4(0.0, 0.0, 0.0, 1.0);
	// ********************************
	// Calculate direction to the light
	// ********************************


	// ***************************
	// Calculate distance to light
	// ***************************

	// ***************************
	// Calculate attenuation value
	// ***************************

	// ******************************
	// Calculate spot light intensity
	// ******************************

	// **********************
	// Calculate light colour
	// **********************

	// ******************************************************************************
	// Now use standard phong shading but using calculated light colour and direction
	// - note no ambient
	// ******************************************************************************


	colour.a = 1.0;

	return colour;
}