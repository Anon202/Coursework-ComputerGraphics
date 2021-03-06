#include <graphics_framework.h>
#include <glm\glm.hpp>
#include <memory>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh m;
effect eff;
target_camera cam;
texture tex;

bool load_content()
{
	// Construct geometry object
	geometry geom;
	geom.set_type(GL_TRIANGLES);
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
	// *****************************************************
	// Define texture coordinates for quad - remember repeat
	// *****************************************************

	vector<vec2> tex_coords
	{
		vec2(0.0f, 0.0f),
		vec2(5.0f, 0.0f),
		vec2(1.0, 1.0f),

		vec2(1.0f, 1.0f),
		vec2(5.0f, 0.0f),
		vec2(5.0f, 1.0f),
		   
		vec2(5.0f, 1.0f),
		vec2(5.0f, 0.0f),
		vec2(6.0f, 0.0f),
		   
		   
		vec2(5.0f, 1.0f),  // end on
		vec2(6.0, 0.0f),
		vec2(6.0f, 0.0f),
		   
		   
		vec2(1.0f, 1.0f), // back side
		vec2(5.0f, 0.0f),
		vec2(0.0f, 0.0f),
		   
		vec2(5.0f, 1.0f),
		vec2(5.0f, 0.0f),
		vec2(1.0f, 1.0f),
		   
		vec2(6.0f, 0.0f),
		vec2(5.0f, 0.0f),
		vec2(5.0f, 1.0f),
		   
		   
		vec2(0.0f, 0.0f),  // end on
		vec2(0.0, 0.0f),
		vec2(1.0f, 1.0f),
		   
		vec2(6.0, 0.0),  			// bottom
		vec2(0.0, 0.0),
		vec2(0.0, 0.0),
		vec2(0.0, 0.0),
		vec2(6.0, 0.0),
		vec2(6.0, 0.0),

	};

	// Add to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);

	// *****************************************
	// Add texture coordinate buffer to geometry
	// *****************************************
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);


	// Create mesh object
	m = mesh(geom);

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

	// Set camera properties
	cam.set_position(vec3(10.0f, 10.0f, 10.0f));
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

	// ************************
	// Bind texture to renderer
	// ************************
	renderer::bind(tex, 0);

	// *****************************************
	// Set the texture value for the shader here
	// *****************************************
	glUniform1i(eff.get_uniform_location("tex"), 0);

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