#version 440

// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;
// Material colour
uniform vec4 material_colour;
// Light colour
uniform vec4 light_colour;
// Direction of the light
uniform vec3 light_dir;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 2) in vec3 normal;

// Outgoing vertex colour
layout (location = 0) out vec4 vertex_colour;

void main()
{
	// ******************
	// Calculate position
	// ******************
	gl_Position = MVP * vec4(position, 1.0f);

	// ********************
	// Transform the normal
	// ********************
	vec3 transformed_normal = N * normal;
	transformed_normal = normalize(transformed_normal);
	
	// ***************************
	// Calculate diffuse component
	// - use transformed normal
	// ***************************
	// Calculate k

	vec3 light_dir_norm = normalize(light_dir);
	float k = dot(transformed_normal, light_dir_norm);
	k = max(k, 0);
	// Calculate diffuse
	
	vec4 amb = material_colour * light_colour;
	vec4 diffuse = k * amb;
	// Ensure diffuse alpha is 1
	diffuse.a = 1.0;

	// ***********************************
	// Output vertex colour - just diffuse
	// ***********************************
	vertex_colour = diffuse;
	
}