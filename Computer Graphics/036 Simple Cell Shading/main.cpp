#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh m;
effect eff;
target_camera cam;
texture tex;

const int subdivisions = 5;

void divide_triangle(const vector<vec3> &points, int divisions, vector<vec3> &positions)
{
	// do we have more divisions to do?
	if (divisions > 0)
	{
		// Calculate new vertices to work on
		vector<vec3> v
		{
			normalize(points[0] + points[1]),
			normalize(points[0] + points[2]),
			normalize(points[1] + points[2])
		};
		// Divide new triangles
		divide_triangle({ points[0], v[0], v[1] }, divisions - 1, positions);
		divide_triangle({ points[2], v[1], v[2] }, divisions - 1, positions);
		divide_triangle({ points[1], v[2], v[0] }, divisions - 1, positions);
		divide_triangle({ v[0], v[2], v[1] }, divisions - 1, positions);
	}
	else
		positions.insert(positions.end(), points.begin(), points.end());
}

bool load_content()
{
	// Construct geometry object
	geometry geom;
	// Required buffers
	vector<vec3> positions;
	vector<vec3> normals;
	// Define the initial tetrahedron - 4 points
	vector<vec3> v
	{
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.942809f, -0.333333f),
		vec3(-0.816497f, -0.471405f, -0.333333f),
		vec3(0.816497f, -0.471405f, 0.333333f)
	};
	// Divide the triangles
	divide_triangle({ v[0], v[1], v[2] }, subdivisions, positions);
	divide_triangle({ v[3], v[2], v[1] }, subdivisions, positions);
	divide_triangle({ v[0], v[3], v[1] }, subdivisions, positions);
	divide_triangle({ v[0], v[2], v[3] }, subdivisions, positions);
	// Copy positions (already normalized) into normals buffer
	normals.insert(normals.end(), positions.begin(), positions.end());
	// Add buffers to geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
	
	// Create mesh object
	m = mesh(geom);

	// **************************
	// Load in simple cell shader
	// **************************
	

    // ************
	// Build effect
    // ************
	

	// ****************************
	// Create vector of colour data
	// ****************************
	

	// *****************
	// Create 1D texture
	// *****************
	

	// Set camera properties
	cam.set_position(vec3(10.0f, 10.0f, 10.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	return true;
}

bool update(float delta_time)
{
	// Update camera
	cam.update(delta_time);
	return true;
}

bool render()
{
	// Bind effect
	renderer::bind(eff);
	// Create MVP matrix
	auto M = m.get_transform().get_transform_matrix();
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MVP = P * V * M;
	// Set MVP matrix uniform
	glUniformMatrix4fv(
		eff.get_uniform_location("MVP"), // Location of uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(MVP)); // Pointer to matrix data

	// ****************
	// Bind the texture
	// ****************
	

	// **********************************
	// Set the uniform values for texture
	// **********************************
	

	// Render the mesh
	renderer::render(m);

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