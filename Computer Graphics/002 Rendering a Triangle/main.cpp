#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
free_camera cam;

// initialise params
GLFWwindow* window;
double xpos = 0;
double ypos = 0;

double current_x = 0;
double current_y = 0;

double new_x = 0;
double new_y = 0;

bool firstMouse = true;

bool initialise()
{
	// ********************************
	// Set input mode - hide the cursor
	// ********************************
	window = renderer::get_window();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ******************************
	// Capture initial mouse position
	// ******************************
	glfwGetCursorPos(window, &xpos, &ypos);


	return true;
}

bool load_content()
{
	// Create triangle data
	// Positions
	vector<vec3> positions
	{
		vec3(0.0f, 0.0f, 1.0f),  // first side
		vec3(5.0f, 0.0f, 1.0f),
		vec3(1.0f, 1.0f, 0.0f),

		vec3(1.0f, 1.0f, 0.0f),
		vec3(5.0f, 0.0f, 1.0f),
		vec3(5.0f, 1.0f, 0.0f),

		vec3(5.0f, 1.0f, 0.0f),
		vec3(5.0f, 0.0f, 1.0f),
		vec3(6.0f, 0.0f, 1.0f),

		
		vec3(5.0f, 1.0f, 0.0f),  // end on
		vec3(6.0, 0.0f, 1.0f),
		vec3(6.0f, 0.0f, -1.0f),
		
		
		vec3(1.0f, 1.0f, 0.0f), // back side
		vec3(5.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, -1.0f),

		vec3(5.0f, 1.0f, 0.0f),
		vec3(5.0f, 0.0f, -1.0f),
		vec3(1.0f, 1.0f, 0.0f),

		vec3(6.0f, 0.0f, -1.0f),		
		vec3(5.0f, 0.0f, -1.0f),
		vec3(5.0f, 1.0f, 0.0f),


		vec3(0.0f, 0.0f, -1.0f),  // end on
		vec3(0.0, 0.0f, 1.0f),
		vec3(1.0f, 1.0f, 0.0f),

		vec3(6.0, 0.0, 1.0),  			// bottom
		vec3(0.0, 0.0, 1.0),
		vec3(0.0, 0.0, -1.0),
		vec3(0.0, 0.0, -1.0),
		vec3(6.0, 0.0, -1.0),
		vec3(6.0, 0.0, 1.0),

		

	};
	// Colours
	vector<vec4> colours
	{
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f)
	};
	// Add to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

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
	cam.set_position(vec3(5.0f, 0.0f, 10.0f));
	cam.set_target(vec3(0.0f, 0.0f, -1.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
	return true;
}

bool update(float delta_time)
{

	// The ratio of pixels to rotation - remember the fov
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());

	// *******************************
	// Get the current cursor position
	// *******************************
	glfwGetCursorPos(window, &new_x, &new_y);


	// ***************************************************
	// Calculate delta of cursor positions from last frame
	// ***************************************************
	/*if (firstMouse)
	{
	current_x = xpos;
	current_y = ypos;
	firstMouse = false;
	}*/

	double delta_x = new_x - current_x;
	double delta_y = new_y - current_y;

	// *************************************************************
	// Multiply deltas by ratios - gets actual change in orientation
	// *************************************************************
	delta_x *= ratio_width;
	delta_y *= -ratio_height;

	// *************************
	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	// *************************
	cam.rotate((float)delta_x, (float)delta_y);

	// *******************************
	// Use keyboard to move the camera
	// - WSAD
	// *******************************
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
		cam.move(vec3(0.0f, 0.0f, 1.0f)*delta_time*10.0f);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
		cam.move(vec3(-1.0f, 0.0f, 0.0f)*delta_time*10.0f);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
		cam.move(vec3(1.0f, 0.0f, 0.0f)*delta_time*10.0f);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
		cam.move(vec3(0.0f, 0.0f, -1.0f)*delta_time*10.0f);


	// ***********
	// Move camera
	// ***********

	// *****************
	// Update the camera
	// *****************
	cam.update(delta_time);


	// *****************
	// Update cursor pos
	// *****************
	glfwGetCursorPos(window, &current_x, &current_y);


	return true;
}

bool render()
{
	// Bind effect
	renderer::bind(eff);
	// Create MVP matrix
	mat4 M(1.0f);
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
	application.set_initialise(initialise);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}