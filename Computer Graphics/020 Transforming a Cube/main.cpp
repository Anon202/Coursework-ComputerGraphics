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
vec3 pos(0.0f, 0.0f, 0.0f);
float s = 1.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_0) && GLFW_PRESS)   // scale up
		s *= 5.0f;

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_P) && GLFW_PRESS)
		s /= 5.0f;
}

bool load_content()
{
	// Create cube data - twelve triangles triangles
	
	geom.set_type(GL_QUADS);
	
	// Positions
	vector<vec3> positions
	{

		// ****************************************
		// Add the position data for triangles here
		// ****************************************
		// Front

		vec3(-1.0f, 1.0f, 0.0f),    // 2
		vec3(-1.0f, -1.0f, 0.0f),	// 0
		vec3(1.0f, -1.0f, 0.0f),	// 1
		vec3(1.0f, 1.0f, 0.0f),		// 3
	
	
		// Back
		vec3(-1.0f, 1.0f, -2.0f),	// 6
		vec3(1.0f, 1.0f, -2.0f),	// 7
		vec3(1.0f, -1.0f, -2.0f),	// 5
		vec3(-1.0f, -1.0f, -2.0f),	// 4	


		// Right
		vec3(1.0f, 1.0f, -2.0f),	// 7
		vec3(1.0f, 1.0f, 0.0f),		// 3
		vec3(1.0f, -1.0f, 0.0f),	// 1
		vec3(1.0f, -1.0f, -2.0f),	// 5


		// Left
		vec3(-1.0f, 1.0f, 0.0f),    // 2
		vec3(-1.0f, 1.0f, -2.0f),	// 6
		vec3(-1.0f, -1.0f, -2.0f),	// 4	
		vec3(-1.0f, -1.0f, 0.0f),	// 0

		// Top
		vec3(-1.0f, -1.0f, 0.0f),	// 0
		vec3(-1.0f, -1.0f, -2.0f),	// 4
		vec3(1.0f, -1.0f, -2.0f),	// 5
		vec3(1.0f, -1.0f, 0.0f),	// 1

		// Bottom
		vec3(-1.0f, 1.0f, -2.0f),	// 6
		vec3(-1.0f, 1.0f, 0.0f),    // 2
		vec3(1.0f, 1.0f, 0.0f),		// 3
		vec3(1.0f, 1.0f, -2.0f),	// 7
	};

	// Colours
	vector<vec4> colours;
	for (auto i = 0; i < 9; ++i) //positions.size(); ++i)
		colours.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	for (auto i = 9; i < 16; ++i)
		colours.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	for (auto i = 16; i < positions.size(); ++i)
		colours.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));

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
	cam.set_position(vec3(10.0f, 10.0f, 10.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	glfwSetKeyCallback(renderer::get_window(), key_callback);

	return true;
}



bool update(float delta_time)
{
	// ***********************************
	// Use keys to update transform values
	// WSAD - movement
	// Cursor - rotation
	// O decrease scale, P increase scale
	// ***********************************


	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))  // up y
		pos += vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))  // down y
		pos += vec3(0.0f, -1.0f, 0.0f);

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))  // right x
		pos += vec3(1.0f, 0.0f, 0.0f);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))  // left x
		pos += vec3(-1.0f, 0.0f, 0.0f);


	// rotation keys
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
	// ****************************
	// Create transformation matrix
	// ****************************


	// M = T (R X S)
    mat4 T = translate(mat4(1.0f), pos);  // translate pos
	mat4 R = eulerAngleXY(theta, rho);    // rotation
    mat4 S = scale(mat4(1.0f), vec3(s, s, s));  // scale
	mat4 M = T * (R * S);

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