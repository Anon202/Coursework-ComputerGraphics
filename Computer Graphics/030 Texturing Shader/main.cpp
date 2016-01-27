#include <graphics_framework.h>
#include <glm\glm.hpp>
#include <memory>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh m;
mesh m2;
effect eff;
target_camera cam;
texture tex;
texture tex2;


float theta = 0.0f;
float rho = 0.0f;

bool load_content()
{
	// Construct geometry object
	geometry geom;
	geometry geom2;

	geom.set_type(GL_QUADS);
	geom2.set_type(GL_QUADS);
	
	// Create triangle data
	// Positions
	vector<vec3> positions
	{
		vec3(-1.0f, 1.0f, 0.0f),
		vec3(-1.0f, -1.0f, 0.0f),
		vec3(1.0f, -1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f),
		vec3(1.0f, -1.0f, 0.0f),
		vec3(1.0f, -1.0f, -2.0f),
		vec3(1.0f, 1.0f, -2.0f)

		
		
	};

	vector<vec3> pos2
	{
		vec3(-1.0f, 1.0f, -2.0f),
		vec3(-1.0f, 1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f),
		vec3(1.0f, 1.0f, -2.0f)
	};

	/* 
		vec3(-1.0f, 1.0f, -2.0f);
		vec3(-1.0f, -1.0f, 0.0f);
		vec3(1.0f, 1.0f, 0.0f);
		vec3(1.0f, 1.0f, -2.0f);
	*/

	// ***************************************
	// Define texture coordinates for triangle
	// ***************************************
	vector<vec2> tex_coords
	{
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(0.5f, 0.0f),
		vec2(0.5f, 1.0f),
		vec2(0.5f, 1.0f),
		vec2(0.5f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f)

		
	};

	vector<vec2> tex_coordsTOP
	{
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),

	};


	// Add to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom2.add_buffer(pos2, BUFFER_INDEXES::POSITION_BUFFER);

	// *****************************************
	// Add texture coordinate buffer to geometry
	// *****************************************
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	geom2.add_buffer(tex_coordsTOP, BUFFER_INDEXES::TEXTURE_COORDS_0);

	// Create mesh object
	m = mesh(geom);
	m2 = mesh(geom2);

	// ****************************
	// Load in texture shaders here
	// ****************************
	eff.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
	eff.add_shader("..\\resources\\shaders\\simple_texture.frag", GL_FRAGMENT_SHADER);

	// ************
	// Build effect
	// ************
	eff.build();

	// *********************
	// Load texture sign.jpg
	// *********************
	tex = texture("..\\resources\\textures\\sign.jpg");
	tex2 = texture("..\\resources\\textures\\wood.dds");

	// Set camera properties
	cam.set_position(vec3(10.0f, 10.0f, 10.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	return true;
}

bool update(float delta_time)
{

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
		theta -= pi<float>() * delta_time;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
		theta += pi<float>() * delta_time;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT))
		rho -= pi<float>() * delta_time;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT))
		rho += pi<float>() * delta_time;
	// Update the camera
	cam.update(delta_time);
	return true;
}

bool render()
{
	mat4 M = eulerAngleXZ(theta, rho);
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MVP = P * V * M;


	renderer::bind(eff);
	renderer::bind(tex, 0);

	// Set MVP matrix uniform
	glUniformMatrix4fv(
		eff.get_uniform_location("MVP"), // Location of uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(MVP)); // Pointer to matrix data



	glUniform1i(eff.get_uniform_location("tex"), 0);

	renderer::render(m);



#if 1
	renderer::bind(eff);
	renderer::bind(tex2, 1);
	glUniformMatrix4fv(
		eff.get_uniform_location("MVP"), // Location of uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(MVP)); // Pointer to matrix data

	glUniform1i(eff.get_uniform_location("tex"), 1);

	renderer::render(m2);
#endif
	// Bind effect
	
	// Create MVP matrix
	//auto M = m.get_transform().get_transform_matrix();

	//m.get_transform().rotate(angleAxis(theta, vec3(1, 0, 0)));
	//m.get_transform().rotate(angleAxis(rho, vec3(0, 1, 0)));

	// ************************
	// Bind texture to renderer
	// ************************
	//renderer::bind(tex, 0);
	//renderer::bind(tex2, 1);

	// *****************************************
	// Set the texture value for the shader here
	// *****************************************
	//glUniform1i(eff.get_uniform_location("tex"), 0);

	// Render the mesh


	return true;
}

void main()
{
	// Create application
	app application;
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}