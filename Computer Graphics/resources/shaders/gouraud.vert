#version 440

// A directional light structure
struct directional_light
{
	vec4 ambient_intensity;
	vec4 light_colour;
	vec3 light_dir;
};

// A material structure
struct material
{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;
// Directional light for the scene
uniform directional_light light;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 2) in vec3 normal;
// Incoming texture coordinate
layout (location = 10) in vec2 tex_coord_in;

// Outgoing primary colour
layout (location = 0) out vec4 primary;
// Outgoing secondary colour
layout (location = 1) out vec4 secondary;
// Outgoing texture coordinate
layout (location = 2) out vec2 tex_coord_out;

void main()
{
	// ******************
	// Calculate position
	// ******************
	gl_Position = MVP * vec4(position, 1.0);

	// ***************************
	// Calculate ambient component
	// ***************************
	vec4 amb = light.ambient_intensity * mat.diffuse_reflection;
	
	

	// ********************
	// Transform the normal
	// ********************
	vec3 transformed_normal = N * normal;
	transformed_normal = normalize(transformed_normal);

	// ***************************
	// Calculate diffuse component
	// ***************************
	// Calculate k
	
	float k = dot(transformed_normal, light.light_dir);
	k = max(k, 0);

	// Calculate diffuse
	
	vec4 diffuse = k * mat.diffuse_reflection * light.light_colour;

	// **********************************
	// Calculate world position of vertex
	// **********************************
	vec4 world4 = M * vec4(position, 1.0);
	vec3 worldPos = vec3(world4);

	// ************************
	// Calculate view direction
	// ************************
	vec3 view_dir = eye_pos - worldPos;
	view_dir = normalize(view_dir);
	

	// ****************************************************
	// Calculate half vector between view_dir and light_dir
	// ****************************************************
	vec3 halfV = light.light_dir + view_dir;
	halfV = normalize(halfV);

	// ****************************
	// Calculate specular component
	// ****************************
	// Calculate k

	float kSpec = dot(transformed_normal, halfV);
	kSpec = max(kSpec, 0);
	
	// Calculate specular
	vec4 specular = pow(kSpec, mat.shininess) * mat.specular_reflection * light.light_colour; 
	

	// ***********
	// Set primary
	// ***********
	primary = mat.emissive + amb + diffuse;
	

	// *************
	// Set secondary
	// *************
	secondary = specular;

	

	// *****************************************
	// Ensure primary and secondary alphas are 1
	// *****************************************
	primary.a = 1.0f;
	secondary.a = 1.0f;

	// *******************************
	// Pass through texture coordinate
	// *******************************
	tex_coord_out = tex_coord_in;
	
}