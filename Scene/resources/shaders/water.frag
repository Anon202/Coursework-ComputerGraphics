#version 440

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

#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light
{
	vec4 ambient_intensity;
	vec4 light_colour;
	vec3 light_dir;
};
#endif

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

// Directional light for the scene
uniform directional_light light;

uniform point_light point;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 1) in vec3 normal;
// Incoming texture coordinate
layout (location = 2) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

// forward declaration of spotPart
vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour);

void main()
{
	// ***************************
	// Calculate ambient component
	// ***************************
	vec4 ambient = mat.diffuse_reflection * light.ambient_intensity;

	// ***************************
	// Calculate diffuse component
	// ***************************
	
	float dotD = dot(normal, light.light_dir);
	float k = max(dotD, 0);
	vec4 diffuse = mat.diffuse_reflection * light.light_colour * k;

	// ************************
	// Calculate view direction
	// ************************
	vec3 view_dir = normalize(eye_pos - position);
	

	// *********************
	// Calculate half vector
	// *********************
	vec3 halfV = normalize(view_dir + light.light_dir);


	// ****************************
	// Calculate specular component
	// ****************************
	float dotS = dot(halfV, normal);
	float kSpec = max(dotS, 0);
	
	vec4 specular = mat.specular_reflection * light.light_colour * pow(kSpec, mat.shininess);

	// **************
	// Sample texture
	// **************
	vec4 tex_colour = texture2D(tex, tex_coord);

	

	// **********************************
	// Calculate primary colour component
	// **********************************
	vec4 primary = mat.emissive + ambient + diffuse;

	// **********************
	// Calculate final colour
	// - remember alpha 1.0
	// **********************

	//colour = primary*tex_colour + specular;
	colour += calculate_point(point, mat, position, normal, view_dir, tex_colour);
	
	// check 
	//colour = primary*tex_colour;
	
	colour.a = .75f;
	

}