// Point light information
#ifndef POINT_LIGHT
#define POINT_LIGHT
struct point_light
{
	vec4 light_colour;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
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

// Point light calculation
vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
	vec4 colour = vec4(0.0, 0.0, 0.0, 1.0);
	// *******************************************
	// Get distance between point light and vertex
	// *******************************************

	// ****************************
	// Calculate attenuation factor
	// ****************************

	// **********************
	// Calculate light colour
	// **********************

	// *******************
	// Calculate light dir
	// *******************

	// ******************************************************************************
	// Now use standard phong shading but using calculated light colour and direction
	// - note no ambient
	// ******************************************************************************


	colour.a = 1.0;
	return colour;
}