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