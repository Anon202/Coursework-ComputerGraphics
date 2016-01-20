#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
target_camera cam;

// Helper method - adds a triangle to geometry
void triangle(const vector<vec3> &points, vector<vec3> &positions, vector<vec4> &colours)
{
	positions.insert(positions.end(), points.begin(), points.end());
	for (auto i = 0; i < 3; ++i)
		colours.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void divide_triangle(const vector<vec3> &points, unsigned int count, vector<vec3> &positions, vector<vec4> &colours)
{
	// ******************
	// Update this method
	// ******************
	
	if (count > 0)
	{
		vector<vec3> p0
		{
		vec3(points[0]),
		vec3((points[0] + points[1]) * 0.5f),
		vec3((points[2] + points[0]) * 0.5f)
	};

		vector<vec3> p1
		{
			vec3(points[1]),
			vec3((points[2] + points[1]) * 0.5f),
			vec3((points[1] + points[0]) * 0.5f)
		};

		vector<vec3> p2
		{
			vec3(points[2]),
			vec3((points[0] + points[2]) * 0.5f),
			vec3((points[2] + points[1]) * 0.5f)
		};

		count -= 1;
		divide_triangle(p0, count, positions, colours);
		divide_triangle(p1, count, positions, colours);
		divide_triangle(p2, count, positions, colours);
	}
	else
	{												// if count is zero.
		triangle(points, positions, colours);
	}

}

bool load_content()
{
	// Required buffers
	vector<vec3> positions;
	vector<vec4> colours;
	divide_triangle({ vec3(1.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f, 0.0f) }, 3, positions, colours);

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
	cam.set_position(vec3(3.0f, 3.0f, 3.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
	return true;
}

bool update(float delta_time)
{
	// Update the camera
	cam.update(delta_time);
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
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}