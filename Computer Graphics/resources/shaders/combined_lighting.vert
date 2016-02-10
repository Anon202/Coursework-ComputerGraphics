#version 440

// The model transformation
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;
// The ambient intensity of the scene
uniform vec4 ambient_intensity;
// The light colour of the scene
uniform vec4 light_colour;
// Direction of the light
uniform vec3 light_dir;
// The diffuse reflection colour
uniform vec4 diffuse_reflection;
// The specular reflection colour
uniform vec4 specular_reflection;
// Shininess of the object
uniform float shininess;
// Position of the camera
uniform vec3 eye_pos;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normla
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
	// Calculate ambient component
	// ***************************
	vec4 ambient = ambient_intensity * diffuse_reflection;

	// ********************
	// Transform the normal
	// ********************

	mat3 NN = mat3(M);

	vec3 transformed_normal = NN * normal;
	transformed_normal = -normalize(transformed_normal);

	// **********************************
	// Calculate world position of vertex
	// **********************************
	vec3 worldPos = vec3(M * vec4(position, 1.0f));
	//worldPos = normalize(worldPos);

	// ***************************
	// Calculate diffuse component
	// - use transformed normal
	// ***************************
	// Calculate k

	// test light 'pos'
	vec3 light_dir2 = worldPos - light_dir;

	vec3 light_dir_norm = normalize(light_dir2);
	float k = dot(transformed_normal, light_dir_norm);
	k = max(k, 0.0);
	
	// Calculate diffuse
	vec4 diffuse = k * vec4(1.0, 1.0, 1.0, 1.0) ;// (diffuse_reflection * light_colour);



	
	// ************************
	// Calculate view direction
	// ************************
	vec3 view_dir = (eye_pos - vec3(worldPos)) / length(eye_pos - vec3(worldPos));

	view_dir = normalize(view_dir);
	// ****************************************************
	// Calculate half vector between view_dir and light_dir
	// ****************************************************
	vec3 halfVec = (view_dir + light_dir_norm) / length(view_dir + light_dir_norm);

	// ****************************
	// Calculate specular component
	// ****************************
	// Calculate k
	
	float kSpec = dot(transformed_normal, halfVec);
	kSpec = max(kSpec, 0.0);
	
	// Calculate specular
	vec4 specular = pow(kSpec, shininess) * (specular_reflection * light_colour);

	// **************************
	// Output combined components
	// **************************
	
	// Ensure alpha is 1
	vec4 outCol = diffuse + specular + ambient; // + diffuse;
	outCol.a = 1.0f;

	vertex_colour = outCol;
	
}