#include <graphics_framework.h>
#include <glm/glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh sphere;
mesh skybox;
effect eff;
effect sky_eff;
cubemap cube_map;
texture tarnish;
target_camera cam;

bool load_content()
{
    // Create a sphere
    sphere = mesh(geometry_builder::create_sphere(25, 25));
    // ******************************
    // Create box geometry for skybox
    // ******************************
    geometry geom;
    geom.set_type(GL_QUADS);
    vector<vec3> positions
    {

		// Face 4
		vec3(-1.0, 1.0, -1.0), // 5
		vec3(-1.0, 1.0, 1.0), //1
		vec3(-1.0, -1.0, 1.0), // 2
		vec3(-1.0, -1.0, -1.0), //7

		// Face 3
		vec3(1.0, 1.0, 1.0), // 4
		vec3(1.0, 1.0, -1.0), //6
		vec3(1.0, -1.0, -1.0), // 8
		vec3(1.0, -1.0, 1.0), // 3

		// Face 5
		vec3(-1.0, 1.0, -1.0), // 5
		vec3(1.0, 1.0, -1.0), //6
		vec3(1.0, 1.0, 1.0), // 4
		vec3(-1.0, 1.0, 1.0), //1



		// Face 6
		vec3(-1.0, -1.0, 1.0), // 2
		vec3(1.0, -1.0, 1.0), // 3
		vec3(1.0, -1.0, -1.0), // 8
		vec3(-1.0, -1.0, -1.0), //7


		// Face 2
		vec3(-1.0, -1.0, -1.0), //7
		vec3(1.0, -1.0, -1.0),  // 8
		vec3(1.0, 1.0, -1.0),   //6
		vec3(-1.0, 1.0, -1.0),  // 5


		// Face 1
		vec3(1.0, -1.0, 1.0), // 3
		vec3(-1.0, -1.0, 1.0), // 2
		vec3(-1.0, 1.0, 1.0), //1
		vec3(1.0, 1.0, 1.0), // 4

    };
    geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
    skybox = mesh(geom);
    // ***********************************
    // Scale box by 10 - allows a distance
    // ***********************************
	skybox.get_transform().scale = vec3(100, 100, 100);


    // ******************************************************
    // Load the cubemap
    // - create array of six filenames +x, -x, +y, -y, +z, -z
    // ******************************************************
    array<string, 6> filenames =
    {
		"..\\resources\\textures\\cubemaps\\alien\\posx.png",
		"..\\resources\\textures\\cubemaps\\alien\\negx.png",
		"..\\resources\\textures\\cubemaps\\alien\\posy.png",
		"..\\resources\\textures\\cubemaps\\alien\\negy.png",
		"..\\resources\\textures\\cubemaps\\alien\\posz.png",
		"..\\resources\\textures\\cubemaps\\alien\\negz.png"
    };
    // ***************
    // Create cube_map
    // ***************
	cube_map = cubemap(filenames);

    // ***************************
    // Load in tarnish.tga texture
    // ***************************
	tarnish = texture("..\\resources\\textures\\tarnish.tga");

    // ******************************
    // Load in environment map shader
    // ******************************
	eff.add_shader("..\\resources\\shaders\\tarnish.vert", GL_VERTEX_SHADER);
	eff.add_shader("..\\resources\\shaders\\tarnish.frag", GL_FRAGMENT_SHADER);
    
    // Build effect
    eff.build();

    // *********************
    // Load in skybox effect
    // *********************
	sky_eff.add_shader("..\\resources\\shaders\\skybox.vert", GL_VERTEX_SHADER);
	sky_eff.add_shader("..\\resources\\shaders\\skybox.frag", GL_FRAGMENT_SHADER);
    
    // Build effect
    sky_eff.build();

    // Set camera properties
    cam.set_position(vec3(0.0f, 0.0f, 10.0f));
    cam.set_target(vec3(0.0f, 0.0f, 0.0f));
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
    return true;
}

bool update(float delta_time)
{
    cam.update(delta_time);

    // *******************************************************************
    // Set skybox position to camera position (camera in centre of skybox)
    // *******************************************************************
	skybox.get_transform().position = cam.get_position();

    return true;
}

bool render()
{
    // *********************************
    // Disable depth test and depth mask
    // *********************************
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

    // ******************
    // Bind skybox effect
    // ******************
	renderer::bind(sky_eff);

    // ****************************
    // Calculate MVP for the skybox
    // ****************************
	auto M = skybox.get_transform().get_transform_matrix();
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MVP = P * V * M;

    // **********************
    // Set MVP matrix uniform
    // **********************
	glUniformMatrix4fv(sky_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
    
    // *******************
    // Set cubemap uniform
    // *******************
	glUniform1i(sky_eff.get_uniform_location("cubemap"), 0);
    
    // *************
    // Render skybox
    // *************
	renderer::render(skybox);
    
    // ********************************
    // Enable depth test and depth mask
    // ********************************
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

    // Bind effect
    renderer::bind(eff);
    // Create MVP matrix
    M = sphere.get_transform().get_transform_matrix();
    V = cam.get_view();
    P = cam.get_projection();
    MVP = P * V * M;
    // Set MVP matrix uniform
    glUniformMatrix4fv(
        eff.get_uniform_location("MVP"),
        1,
        GL_FALSE,
        value_ptr(MVP));
    // Set M matrix uniform
    glUniformMatrix4fv(
        eff.get_uniform_location("M"),
        1,
        GL_FALSE,
        value_ptr(M));
    // Set N matrix uniform
    glUniformMatrix3fv(
        eff.get_uniform_location("N"),
        1,
        GL_FALSE,
        value_ptr(sphere.get_transform().get_normal_matrix()));
    // Set eye_pos value
    glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

    // *******************
    // Set cubemap uniform
    // *******************
	glUniform1i(eff.get_uniform_location("cubemap"), 1);   // renders when 1 but sphere black

    
    // *******************
    // Set tarnish uniform
    // *******************
	glUniform1i(eff.get_uniform_location("tarnish"), 0); 
    

    // Render mesh
    renderer::render(sphere);

    return true;
}

void main()
{
    // Create application
    app application;
    // Set methods
    application.set_load_content(load_content);
    application.set_update(update);
    application.set_render(render);
    // Run application
    application.run();
}