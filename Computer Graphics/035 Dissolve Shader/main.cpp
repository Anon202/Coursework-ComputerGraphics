#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh m;
effect eff;
target_camera cam;
// Main texture
texture tex;
// Dissolve texture
texture dissolve;
// Dissolve factor to set on shader
float dissolve_factor = 1.0f;

bool load_content()
{
	// Construct geometry object
	geometry geom;
	// Create cube data - twelve triangles triangles
	// Positions
	vector<vec3> positions
	{
		// Front
		vec3(1.0f, 1.0f, 1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		vec3(1.0f, -1.0f, 1.0f),
		// Back
		vec3(-1.0f, 1.0f, -1.0f),
		vec3(1.0f, 1.0f, -1.0f),
		vec3(1.0f, -1.0f, -1.0f),
		vec3(-1.0f, 1.0f, -1.0f),
		vec3(1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		// Right
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(-1.0f, 1.0f, -1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		// Left
		vec3(1.0f, 1.0f, -1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, -1.0f, 1.0f),
		vec3(1.0f, 1.0f, -1.0f),
		vec3(1.0f, -1.0f, 1.0f),
		vec3(1.0f, -1.0f, -1.0f),
		// Top
		vec3(1.0f, 1.0f, -1.0f),
		vec3(-1.0f, 1.0f, -1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, -1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		// Bottom
		vec3(1.0f, -1.0f, 1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(1.0f, -1.0f, 1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(1.0f, -1.0f, -1.0f)
	};
	// Texture coordinates
	vector<vec2> tex_coords;
	// Six sides, 6 vertices per side
	for (unsigned int i = 0; i < 6; ++i)
	{
		tex_coords.push_back(vec2(1.0f, 1.0f));
		tex_coords.push_back(vec2(0.0f, 1.0f));
		tex_coords.push_back(vec2(0.0f, 0.0f));
		tex_coords.push_back(vec2(1.0f, 1.0f));
		tex_coords.push_back(vec2(0.0f, 0.0f));
		tex_coords.push_back(vec2(1.0f, 0.0f));
	}
	// Add to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

	// Create mesh object
	m = mesh(geom);
	// Scale geometry
	m.get_transform().scale = vec3(10.0f, 10.0f, 10.0f);

	// ***********************
	// Load in dissolve shader
	// ***********************
	eff.add_shader("..\\resources\\shaders\\dissolve.vert", GL_VERTEX_SHADER);
	eff.add_shader("..\\resources\\shaders\\dissolve.frag", GL_FRAGMENT_SHADER);

    // ************
	// Build effect
    // ************
	eff.build();


	// ****************
	// Load in textures
	// ****************
	tex = texture("..\\resources\\textures\\checked.gif", false, false);
	dissolve = texture("..\\resources\\textures\\dissolve.png", false, false);



	// Set camera properties
	cam.set_position(vec3(30.0f, 30.0f, 30.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	return true;
}

bool update(float delta_time)
{
	// Use up an down to modify the dissolve factor
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
		dissolve_factor = clamp(dissolve_factor + 0.1f * delta_time, 0.0f, 1.0f);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
		dissolve_factor = clamp(dissolve_factor - 0.1f * delta_time, 0.0f, 1.0f);
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

	// *************************************
	// Set the dissolve_factor uniform value
	// *************************************
	glUniform1f(eff.get_uniform_location("dissolve_factor"), dissolve_factor);  // param location/ value for FLOAT


	// ****************************************************
	// Bind the two textures - use different index for each
	// ****************************************************
	renderer::bind(tex, 0);
	renderer::bind(dissolve, 1);


	// *******************************************************
	// Set the uniform values for textures - use correct index
	// *******************************************************
	glUniform1i(eff.get_uniform_location("tex"), 0);
	glUniform1i(eff.get_uniform_location("dissolve"), 1);			// param index as sampler2d


	
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