#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
texture tex;
target_camera cam;

bool load_content()
{
	// Create plane mesh
	meshes["plane"] = mesh(geometry_builder::create_plane());

	// ******************
	// Create meshes here
	// - Box
	// - Tetrahedron
	// - Pyramid
	// - Disk
	// - Cylinder
	// - Sphere
	// - Torus
	// ******************
	vec3 dimen(1.0f, 1.0f, 1.0f);
	vec3 scale(5.0f, 5.0f, 5.0f);

	meshes["box"] = mesh(geometry_builder::create_box(scale));
	meshes["tetrahedron"] = mesh(geometry_builder::create_tetrahedron(scale));
	meshes["pyramid"] = mesh(geometry_builder::create_pyramid(scale));
	meshes["disk"] = mesh(geometry_builder::create_disk(20));
	meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20));
	meshes["sphere"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["torus"] = mesh(geometry_builder::create_torus(20, 20, 1, 5));

	// ***************************************
	// Set the transforms for your meshes here
	// ***************************************
	meshes["box"].get_transform().translate(vec3(-10, 2.5, -30));
	meshes["tetrahedron"].get_transform().translate(vec3(-30, 10, -10));
	meshes["pyramid"].get_transform().translate(vec3(-10, 7.5, -30));

	meshes["disk"].get_transform().scale = vec3(3, 1, 3);
	meshes["disk"].get_transform().translate(vec3(-10, 11.5, -30));
	meshes["disk"].get_transform().rotate(angleAxis(pi<float>()/2, vec3(1, 0, 0)));

	meshes["cylinder"].get_transform().scale = scale;
	meshes["cylinder"].get_transform().translate(vec3(-25, 2.5, -25));

	meshes["sphere"].get_transform().scale = vec3(2.5, 2.5, 2.5);
	meshes["sphere"].get_transform().translate(vec3(-25, 10, -25));

	meshes["torus"].get_transform().translate(vec3(-25, 10, -25));
	meshes["torus"].get_transform().rotate(angleAxis(pi<float>() / 2, vec3(1, 0, 0)));

	// Load texture
	tex = texture("..\\resources\\textures\\checked.gif");

	// Load in shaders
	eff.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
	eff.add_shader("..\\resources\\shaders\\simple_texture.frag", GL_FRAGMENT_SHADER); 
	// Build effect
	eff.build();

	// Set camera properties
	cam.set_position(vec3(10.0f, 10.0f, 10.0f));
	cam.set_target(vec3(-100.0f, 0.0f, -100.0f));
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
	// Render meshes
	for (auto &e : meshes)
	{
		auto m = e.second;
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

		// Bind and set texture
		renderer::bind(tex, 0);
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Render mesh
		renderer::render(m);
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