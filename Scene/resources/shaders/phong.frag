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

// Directional light for the scene
uniform directional_light light;

uniform point_light point[3];

uniform spot_light spot;

// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;

// incoming alphaVal for output pixel.
uniform float alphaVal;

uniform sampler2D ssao;


// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 1) in vec3 normal;
// Incoming texture coordinate
layout (location = 2) in vec2 tex_coord;



// Outgoing colour
layout (location = 0) out vec4 colour;

vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour);

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

vec2 screenCoord()
{
	return vec2(gl_FragCoord);
}

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
	ambient *= texture(ssao, screenCoord()).r;

	vec4 primary = mat.emissive + ambient + diffuse;

	// **********************
	// Calculate final colour
	// - remember alpha 1.0
	// **********************

	colour = tex_colour;

	// if semi-transparent dont use primary lighting
	if (alphaVal >= 1.0)
	{
		colour *= primary;
	}

	colour += specular;
	
	for (int i = 0; i < 3; ++i)
	{
		colour += calculate_point(point[i], mat, position, normal, view_dir, tex_colour);
	}
	
	colour = ambient;//+= calculate_spot(spot, mat, position, normal, view_dir, tex_colour);

	colour.a = alphaVal;

}

