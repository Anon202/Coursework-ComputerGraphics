#version 440

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

// forward declaration of normal method
vec3 calc_normal(in vec3 normal, in vec3 tangent, in vec3 binormal, in vec3 sampled_normal, in vec2 tex_coord);

// Directional light for the scene
uniform directional_light light;

uniform spot_light spot;

// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex[5];

// Incoming position
layout (location = 0) in vec3 position;
// Incoming texture coordinate
layout (location = 1) in vec2 tex_coord;
// Incoming normal
layout (location = 2) in vec3 normal;

layout (location = 4) in vec3 binormal;
// Incoming tangent
layout (location = 3) in vec3 tangent;


// Outgoing colour
layout (location = 0) out vec4 colour;

vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{

	vec4 colour = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 L = normalize(spot.position - position);
	vec3 R = spot.direction;

	// ***************************
	// Calculate distance to light
	// ***************************
	float d = distance(spot.position, position);

	// ***************************
	// Calculate attenuation value
	// ***************************
	float att = spot.constant + spot.linear*d + spot.quadratic*d*d;

	// ******************************
	// Calculate spot light intensity
	// ******************************
	
	
	float intensity = max(dot(-R, L),0);
	intensity = pow(intensity, spot.power);

	// **********************
	// Calculate light colour
	// **********************
	vec4 lightCol = (intensity/att)*spot.light_colour;
	

	// ******************************************************************************
	// Now use standard phong shading but using calculated light colour and direction
	// - note no ambient
	// ******************************************************************************
	float dotD = dot(normal, L);
	float k = max(dotD, 0);
	vec4 diffuse = mat.diffuse_reflection * lightCol * k;
	
	vec3 halfV = normalize(view_dir + L);
	float dotS = dot(halfV, normal);
	float kSpec = max(dotS, 0);
	vec4 specular = mat.specular_reflection * lightCol * pow(kSpec, mat.shininess);

    vec4 primary = mat.emissive + diffuse;

    colour = primary * tex_colour + specular;
  

	return colour;
}


void main()
{
	colour = vec4(0.0, 0.0, 0.0, 1.0); // set alpha as one to ensure opacity full

	// sample all textures
	vec4 tex_colourBottom = texture2D(tex[0], tex_coord);
	vec4 tex_colourBottomNormal = texture2D(tex[1], tex_coord);
	vec4 tex_colourOverlay = texture2D(tex[2], tex_coord);
	vec4 tex_colourOverlayNormal = texture2D(tex[3], tex_coord);
	vec4 tex_colourBlend = texture2D(tex[4], tex_coord);


	// blend normal maps (vec3 as normal)
	vec3 normalMix = mix(tex_colourBottomNormal, tex_colourOverlayNormal, tex_colourBlend.r).xyz;

	// blend decal textures
	vec4 textMix = mix(tex_colourBottom, tex_colourOverlay, tex_colourBlend.r);

	// calcualte normal from the map
	vec3 calcNormal = calc_normal(normal, tangent, binormal, normalMix, tex_coord);
	
	// calculate light from new normal
	vec4 ambient = mat.diffuse_reflection * light.ambient_intensity;
	
	float dotD = dot(calcNormal, light.light_dir);
	float k = max(dotD, 0);
	vec4 diffuse = mat.diffuse_reflection * light.light_colour * k;


	vec3 view_dir = normalize(eye_pos - position);
	
	vec3 halfV = normalize(view_dir + light.light_dir);



	float dotS = dot(halfV, calcNormal);
	float kSpec = max(dotS, 0);
	
	vec4 specular = mat.specular_reflection * light.light_colour * pow(kSpec, mat.shininess);


	vec4 primary = mat.emissive + ambient + diffuse;

	colour = primary* textMix + specular;

}