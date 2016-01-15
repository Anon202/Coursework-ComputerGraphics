#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
target_camera cam;

// create a pos vector for translation
vec3 pos(0.0f, 0.0f, 0.0f);

// create theta
float theta = 0.0f;

// create s ( scale var)
float s = 0.0f;

// create time var for accumulation
float total_time = 0.0f;

const int num_points = 50000;

void create_sierpinski(geometry &geom)
{
	vector<vec3> points;
	vector<vec4> colours;
	// Three corners of the triangle
	array<vec3, 3> v =
	{
		vec3(-1.0f, -1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(1.0f, -1.0f, 0.0f)
	};
	// Create random engine - generates random numbers
	default_random_engine e;
	// Create a distribution.  3 points in array so want 0-2
	uniform_int_distribution<int> dist(0, 2);
	// Add first point to the geometry
	points.push_back(vec3(0.25f, 0.5f, 0.0f));
	// Add first colour to the geometry
	colours.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Add random points using distribution
	for (auto i = 1; i < num_points; ++i)
	{
        // ****************
		// Add random point
        // ****************
		auto n = dist(e);

		points.push_back((points[i - 1] + v[n]) / 2.0f);  // adding a point to the vector, 

        // ***************************
		// Add colour - all points red
        // ***************************

		colours.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));

	}

    // ***********************
	// Add buffers to geometry
    // ***********************
	geom.add_buffer(points, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);


}

bool load_content()
{
	// Set to points type
	geom.set_type(GL_POINTS);
	// Create sierpinski gasket
	create_sierpinski(geom);

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
	cam.set_position(vec3(2.0f, 2.0f, 2.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
	return true;
}

bool update(float delta_time)
{
	// Update the camera
	cam.update(delta_time);


	// Increment theta - half a rotation per second
	theta += pi<float>() * delta_time;

	total_time += delta_time;

	// increment scale each second by sin
	s = 1.0f + sinf(total_time);


	// add direction // get key if key pressed is correct key update pos vector by 5.0f per seconds
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
		pos += vec3(0.0f, 0.0f, 5.0f) * delta_time;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
		pos += vec3(0.0f, 0.0f, -5.0f) * delta_time;

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT))
		pos += vec3(5.0f, 0.0f, 0.0f) * delta_time;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT))
		pos += vec3(-5.0f, 0.0f, 0.0f) * delta_time;



	return true;
}

bool render()
{
	// Bind effect
	renderer::bind(eff);
	

	// change M from identity to transformation matrix
	mat4 T = translate(mat4(1.0f), pos);
	
	// angle, axis
	mat4 R = rotate(mat4(1.0f), theta, vec3(1.0f, 0.0f, 0.0f));
	

	// scale
	mat4 S = scale(mat4(1.0f), vec3(s, s, s));
	
	mat4 M = T * (S*R);

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