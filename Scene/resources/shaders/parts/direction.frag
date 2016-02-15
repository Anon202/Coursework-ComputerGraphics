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
	vec4 ambient = mat.diffuse_reflection * light.ambient_intensity;


	// ***************************
	// Calculate diffuse component
	// ***************************
	float k = dot(normal, light.light_dir);
	k = max(k, 0.0);
	vec4 diffuse = mat.diffuse_reflection * light.light_colour * k;

	// *********************
	// Calculate half vector
	// *********************
	vec3 halfV = normalize(view_dir + light.light_dir);

	// ****************************
	// Calculate specular component
	// ****************************
	float kSpec = dot(halfV, normal);
	kSpec = max(kSpec, 0.0);

	vec4 specular = mat.specular_reflection * light.light_colour * pow(kSpec, mat.shininess);


	vec4 primary = mat.emissive + ambient + diffuse;

	// **************************
	// Calculate colour to return
	// - remember alpha = 1
	// **************************
	colour = tex_colour*primary + specular;
	colour.a = 1.0f;
	// *************
	// Return colour
	// *************
	return colour;
}