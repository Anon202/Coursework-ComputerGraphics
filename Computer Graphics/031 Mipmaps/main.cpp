#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

// Two meshes
array<mesh*, 2> meshes;
effect eff;
target_camera cam;
// Two textures
array<texture, 2> texs;

bool load_content()
{
	// Construct geometry object
	geometry geom;
	geom.set_type(GL_QUADS);
	// Create quad data
	// Positions
	vector<vec3> positions
	{
		vec3(-1.0f, 1.0f, 0.0f),
		vec3(-1.0f, -1.0f, 0.0f),
		vec3(1.0f, -1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f)
	};
	// Texture coordinates
	vector<vec2> tex_coords
	{
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f)
	};
	// Add to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

	// ****************************************
	// Create two mesh objects - reuse geometry
	// ****************************************
	meshes[0] = new mesh(geom);
	meshes[1] = new mesh(geom);

    // *********************
	// Scale each mesh by 10
    // *********************
	vec3 a(2.0f, 2.0f, 2.0f);
	for (int i = 0; i < 2; ++i)
		meshes[i]->get_transform().scale = a;

	// ***********************************
	// Set mesh positions - remember scale
	// ***********************************
	vec3 b(10.0f, 0.0f, 0.0f);
	meshes[0]->get_transform().translate(b);

	vec3 c(6.0f, 0.0f, 0.0f);
	meshes[1]->get_transform().translate(c);


	// Load in texture shaders
	eff.add_shader(
		"..\\resources\\shaders\\simple_texture.vert", // filename
		GL_VERTEX_SHADER); // type
	eff.add_shader(
		"..\\resources\\shaders\\simple_texture.frag", // filename
		GL_FRAGMENT_SHADER); // type

	// Build effect
	eff.build();

	// *********************
	// Load texture sign.jpg
	// 0 - no mipmaps
	// 1 - mipmaps
	// *********************
	texs[0] = texture("..\\resources\\textures\\sign.jpg", true, false);
	texs[1] = texture("..\\resources\\textures\\sign.jpg", false, false);


	// Set camera properties
	cam.set_position(vec3((10.0f * (meshes.size() - 1)), 0.0f, 20.0f));
	cam.set_target(vec3((10.0f * (meshes.size() - 1)), 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	return true;
}

bool update(float delta_time)
{
	// Move camera with cursor
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
		cam.set_position(cam.get_position() - vec3(0.0f, 0.0f, 10.0f) * delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
		cam.set_position(cam.get_position() + vec3(0.0f, 0.0f, 10.0f) * delta_time);
	// Update the camera
	cam.update(delta_time);
	return true;
}

bool render()
{
	// Bind effect
	renderer::bind(eff);
	// Render each mesh
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		// Create MVP matrix
		auto M = meshes[i]->get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(
			eff.get_uniform_location("MVP"), // Location of uniform
			1, // Number of values - 1 mat4
			GL_FALSE, // Transpose the matrix?
			value_ptr(MVP)); // Pointer to matrix data

		// ********************************
		// Bind correct texture to renderer
		// ********************************
		renderer::bind(texs[i], i);

		// *****************************************
		// Set the texture value for the shader here
		// *****************************************
		glUniform1i(eff.get_uniform_location("tex"), i);

		// Render the mesh
		renderer::render(*meshes[i]);
	}

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