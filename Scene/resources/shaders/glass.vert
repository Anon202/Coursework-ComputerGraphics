#version 440

#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light		// dir light structure
{
	vec4 ambient_intensity;
	vec4 light_colour;
	vec3 light_dir;
};
#endif

#ifndef MATERIAL
#define MATERIAL
struct material					// material structure
{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};
#endif

uniform mat4 MVP; // Model View Projection matrix

uniform mat4 M;	  // model matrix

uniform mat3 N;   // normal matrix 3x3

uniform directional_light light; // light for the scene

uniform material mat; // material of object

uniform vec3 eye_pos; // for specular calc


layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 10) in vec2 tex_coord_in;

layout (location = 0) out vec4 out_primary;
layout (location = 1) out vec4 out_secondary;
layout (location = 2) out vec2 tex_coord_out;

void main()
{
	// Calculate screen position of vertex
	gl_Position = MVP * vec4(position, 1.0);
	
	// transform the normals
	vec3 transformedN = N * normal;

	// calculate ambient
	vec4 ambient = light.ambient_intensity * mat.diffuse_reflection;

	// calculate diffuse
	float dotTemp = dot(light.light_dir, transformedN);
	float k = max(dotTemp, 0);
	vec4 diffuse = mat.diffuse_reflection * light.light_colour * k;

	// calculate specular
	vec3 view_dir = normalize(eye_pos - position);
	vec3 halfV = normalize(view_dir + light.light_dir);

	dotTemp = dot(halfV, transformedN);
	k = max(dotTemp, 0);

	vec4 specular = mat.specular_reflection * light.light_colour * pow(k, mat.shininess);

	// Output colour to the fragment shader
	out_primary = ambient + diffuse;
	out_primary.a = 1.0f;
	out_secondary = specular;
	out_secondary.a = 1.0f;
	tex_coord_out = tex_coord_in;
}