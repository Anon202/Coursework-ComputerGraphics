#version 440

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;
// Material colour
uniform vec4 material_colour;
// Shininess of the object
uniform float shininess;
// Light colour
uniform vec4 light_colour;
// Direction of the light
uniform vec3 light_dir;
// Position of the camera
uniform vec3 eye_pos;

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
	gl_Position = MVP * vec4(position, 1.0);

	// ********************
	// Transform the normal
	// ********************
	vec3 transformed_normal =  normal;
	transformed_normal = normalize(transformed_normal);
	
	// ************************
	// Calculate world position
	// ************************
	vec4 worldPos4 = M * vec4(position, 1.0);
	vec3 worldPos = vec3(worldPos4);

	// ************************
	// Calculate view direction
	// ************************
	vec3 view_dir = (eye_pos - worldPos);

	view_dir = view_dir/length(view_dir);

	// ****************************************************
	// Calculate half vector between view_dir and light_dir
	// ****************************************************
	vec3 viewLight = view_dir + light_dir;// vec4(light_dir, 1);
	
	vec3 half_vec = viewLight/length(viewLight);

	// ****************************
	// Calculate specular component
	// ****************************
	// Calculate k

	float k = dot(transformed_normal, half_vec);
	k = max(k, 0);

	// Calculate specular
	vec4 amb = light_colour * material_colour;

	vec4 specular = amb * pow(k,shininess);
	// Ensure alpha is 1
	specular.a = 1.0f;

	// ***********************************
	// Output vertex colour - just diffuse
	// ***********************************
	vertex_colour = specular;
	
}