#version 440

// The transformation matrix
uniform mat4 MVP;
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
	
	
	// ***************************
	// Calculate diffuse component
	// ***************************
	// Calculate k
	vec4 k = material_colour * light_colour;


	// Calculate diffuse
	vec3 normal2 = normalize(normal);
	vec3 light_dir2 = normalize(light_dir);

	float dot2 = dot(normal2, light_dir2);
	dot2 = max(dot2, 0);
	// Ensure diffuse alpha is 1
	
	vec4 diffuse = k * dot2;


	// ***********************************
	// Output vertex colour - just diffuse
	// ***********************************
	vertex_colour = diffuse;
	vertex_colour.a = 1.0;

}