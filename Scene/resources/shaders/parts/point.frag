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
	// *******************************************
	// Get distance between point light and vertex
	// *******************************************
	float d = distance(point.position, position);

	// ****************************
	// Calculate attenuation factor
	// ****************************	
	float att = point.constant + point.linear*d + point.quadratic*d*d;

	// **********************
	// Calculate light colour
	// **********************
	vec4 lightCol = point.light_colour * (1 / att);


	// *******************
	// Calculate light dir
	// *******************

	vec3 light_dir = normalize(point.position - position);

	// ******************************************************************************
	// Now use standard phong shading but using calculated light colour and direction
	// - note no ambient
	// ******************************************************************************
	float dotD = dot(normal, light_dir);
	float k = max(dotD, 0);
	vec4 diffuse = mat.diffuse_reflection * lightCol * k;

	vec3 halfV = normalize(view_dir + light_dir);
	float dotS = dot(halfV, normal);
	float kSpec = max(dotS, 0);
	vec4 specular = mat.specular_reflection * lightCol * pow(kSpec, mat.shininess);


	vec4 primary = mat.emissive + diffuse;

	vec4 colour = primary * tex_colour + specular;


	return colour;
}