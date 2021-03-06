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
uniform point_light point;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;


// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 1) in vec3 normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main()
{

	// ***************************
	// Calculate diffuse component
	// ***************************

	float d = distance(point.position, position);
	float att = point.constant + point.linear*d + point.quadratic*d*d;
	vec4 lightCol = point.light_colour * (1/att);

	vec3 light_dir = normalize(point.position - position);

	float dotD = dot(normal, light_dir);
	float k = max(dotD, 0);
	vec4 diffuse = mat.diffuse_reflection * lightCol * k;

	// ************************
	// Calculate view direction
	// ************************
	vec3 view_dir = normalize(eye_pos - position);


	// *********************
	// Calculate half vector
	// *********************
	vec3 halfV = normalize(view_dir + light_dir);


	// ****************************
	// Calculate specular component
	// ****************************
	float dotS = dot(halfV, normal);
	float kSpec = max(dotS, 0);

	vec4 specular = mat.specular_reflection * lightCol * pow(kSpec, mat.shininess);

	// **************
	// Sample texture
	// **************
	vec4 tex_colour = texture2D(tex, tex_coord);



	// **********************************
	// Calculate primary colour component
	// **********************************
	vec4 primary = mat.emissive + diffuse;

	// **********************
	// Calculate final colour
	// - remember alpha 1.0
	// **********************

	colour = primary * tex_colour + specular;

	// check 
	//colour = primary*tex_colour;

	colour.a = 1.0f;


}