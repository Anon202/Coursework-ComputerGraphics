#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

// Four meshes
array<mesh*, 4> meshes;
effect eff;
target_camera cam;
// Four textures
array<texture*, 4> texs;

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
	for (int i = 0; i < 4; ++i)
		meshes[i] = new mesh(geom);
	

    // *********************
	// Scale each mesh by 10
    // *********************
	vec3 a(2.0f, 2.0f, 2.0f);

	for (int i = 0; i < 4; ++i)
		meshes[i]->get_transform().scale = a;

	// ***********************************
	// Set mesh positions - remember scale
	// ***********************************
	//meshes[0]->get_transform().translate(vec3(10.0f, 10.0f, 10.0f));


	vec3 b[4]
	{

		vec3(22.0f, 0.0f, 0.0f),
		vec3(26.0f, 0.0f, 0.0f),
		vec3(30.0f, 0.0f, 0.0f),
		vec3(34.0f, 0.0f, 0.0f),
	};
	for (int i = 0; i < 4; ++i)
	{
		meshes[i]->get_transform().translate(b[i]);
	}

	// Load in texture shaders
	eff.add_shader(
		"..\\resources\\shaders\\simple_texture.vert", // filename
		GL_VERTEX_SHADER); // type
	eff.add_shader(
		"..\\resources\\shaders\\simple_texture.frag", // filename
		GL_FRAGMENT_SHADER); // type

	// Build effect
	eff.build();

	// ******************************
	// Load textures sign.jpg
	// 0 - no mipmaps, no anisotropic
	// 1 - mipmaps, no anisotropic
	// 2 - no mipmaps, anisotropic
	// 3 - mipmaps, anisotropic
	// ******************************
	texs[0] = new texture("..\\resources\\textures\\sign.jpg", false, false);
	texs[1] = new texture("..\\resources\\textures\\sign.jpg", false, true);
	texs[2] = new texture("..\\resources\\textures\\sign.jpg", true, false);
	texs[3] = new texture("..\\resources\\textures\\sign.jpg", true, true);



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
		renderer::bind(*texs[i], i);

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