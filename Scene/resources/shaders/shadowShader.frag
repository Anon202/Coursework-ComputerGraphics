#version 440

// This shader requires spot.frag, shadow.frag


#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light
{
	vec4 ambient_intensity;
	vec4 light_colour;
	vec3 light_dir;
};
#endif

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

// Forward declarations of used functions
vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour);
float calculate_shadow(in sampler2D shadow_map, in vec4 light_space_pos);

uniform directional_light light;
// Spot light being used in the scene
uniform spot_light spot;
// Material of the object being rendered
uniform material mat;
// Position of the eye
uniform vec3 eye_pos;
// Texture to sample from
uniform sampler2D tex;
// Shadow map to sample from
uniform sampler2D shadow_map;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 1) in vec3 normal;
// Incoming texture coordinate
layout (location = 2) in vec2 tex_coord;
// Incoming light space position
layout (location = 3) in vec4 light_space_pos;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
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

    // **********************
    // Calculate shade factor
    // **********************
    float shade = calculate_shadow(shadow_map, light_space_pos);

    // **************
    // Sample texture
    // **************
    vec4 tex_colour = texture(tex, tex_coord);


	// **********************
	// Calculate final colour
	// - remember alpha 1.0
	// **********************

	// without ambient for shade 
	vec4 primary = mat.emissive + diffuse;



	colour = primary*tex_colour + specular;
	
	// ********************
    // Calculate spot light
    // ********************

    colour += calculate_spot(spot, mat, position, normal, view_dir, tex_colour);


	// *********************
	// Scale colour by shade
	// *********************

	colour *= shade;
	colour += ambient*tex_colour;
	colour.a = 1.0;
}