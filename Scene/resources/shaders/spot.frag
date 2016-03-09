#version 440

// Spot light data
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

// Material data
struct material
{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};

// Spot light being used in the scene
uniform spot_light spot;
// Material of the object being rendered
uniform material mat;
// Position of the eye (camera)
uniform vec3 eye_pos;
// Texture to sample from
uniform sampler2D tex;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 1) in vec3 normal;
// Incoming texture coordinate
layout (location = 2) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
	vec3 L = normalize(spot.position - position);
	vec3 R = spot.direction;
	
    vec3 view_dir = normalize(eye_pos - position);

	// **************
	// Sample texture
	// **************
	vec4 tex_colour = texture2D(tex, tex_coord);

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


}