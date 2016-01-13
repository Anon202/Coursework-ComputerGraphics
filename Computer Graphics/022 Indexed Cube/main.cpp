#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
target_camera cam;
float theta = 0.0f;
float rho = 0.0f;

bool load_content()
{
	// Create cube data - eight corners
	// Positions
	vector<vec3> positions
	{
		// *******************************************
		// Add the position data for cube corners here
		// *******************************************

	};

	// Colours
	vector<vec4> colours;
	for (auto i = 0; i < positions.size(); ++i)
		colours.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Create the index buffer
	vector<GLuint> indices
	{
		// *********************************************************
		// Add index information here - 3 per triangle, 12 triangles
		// *********************************************************
		// Front

		// Back

		// Right

		// Left

		// Top

		// Bottom

	};

	// Add to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

	// ****************************
	// Add index buffer to geometry
	// ****************************
	

	// Load in shaders
	eff.add_shader(
		"..\\resources\\shaders\\basic.vert", // filename
		GL_VERTEX_SHADER); // type
	eff.add_shader(
		"..\\resources\\shaders\\basic.frag", // filename
		GL_FRAGMENT_SHADER); // type
	// Build effect
	eff.build();

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
	// Bind effect
	renderer::bind(eff);
	// Create MVP matrix
	mat4 M = eulerAngleXZ(theta, rho);
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MVP = P * V * M;
	// Set MVP matrix uniform
	glUniformMatrix4fv(
		eff.get_uniform_location("MVP"), // Location of uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(MVP)); // Pointer to matrix data
	// Render geometry
	renderer::render(geom);
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