#include <graphics_framework.h>
#include <glm/glm.hpp>
#include "gui.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh sphere;
mesh skybox;
effect eff;
effect sky_eff;
cubemap cube_map;
free_camera cam;


// initialise params
GLFWwindow* window;
double xpos = 0;
double ypos = 0;

double current_x = 0;
double current_y = 0;

double new_x = 0;
double new_y = 0;

bool firstMouse = true;

bool showGUI;

bool initialise()
{
	// ********************************
	// Set input mode - hide the cursor
	// ********************************
	window = renderer::get_window();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ******************************
	// Capture initial mouse position
	// ******************************
	glfwGetCursorPos(window, &xpos, &ypos);
	showGUI = true;
	initUI(window);

	return true;
}

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
    // Scale box by 100 - allows a distance
    // ***********************************
	skybox.get_transform().scale = vec3(100, 100, 100);

    // ******************************************************
    // Load the cubemap
    // - create array of six filenames +x, -x, +y, -y, +z, -z
    // ******************************************************
	array<string, 6> filenames =
	{
		"..\\resources\\textures\\cubemaps\\valley\\posx.tga",
		"..\\resources\\textures\\cubemaps\\valley\\negx.tga",
		"..\\resources\\textures\\cubemaps\\valley\\posy.tga",
		"..\\resources\\textures\\cubemaps\\valley\\negy.tga",
		"..\\resources\\textures\\cubemaps\\valley\\posz.tga",
		"..\\resources\\textures\\cubemaps\\valley\\negz.tga"
	};

    // ***************
    // Create cube_map
    // ***************

	cube_map = cubemap(filenames);

    // Load in shaders
    eff.add_shader("shader.vert", GL_VERTEX_SHADER);
    eff.add_shader("shader.frag", GL_FRAGMENT_SHADER);
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

	// The ratio of pixels to rotation - remember the fov
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());

	// *******************************
	// Get the current cursor position
	// *******************************
	glfwGetCursorPos(window, &new_x, &new_y);


	// ***************************************************
	// Calculate delta of cursor positions from last frame
	// ***************************************************
	if (firstMouse)
	{
		current_x = xpos;
		current_y = ypos;
		firstMouse = false;
	}

	double delta_x = new_x - current_x;
	double delta_y = new_y - current_y;

	// *************************************************************
	// Multiply deltas by ratios - gets actual change in orientation
	// *************************************************************
	delta_x *= ratio_width;
	delta_y *= -ratio_height;


	// *************************
	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	// *************************
	cam.rotate((float)delta_x, (float)delta_y);


	// *******************************
	// Use keyboard to move the camera
	// - WSAD
	// *******************************
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
	{
		cam.move(vec3(0.0f, 0.0f, 1.0f));
		//skybox.get_transform().translate(vec3(0.0f, 0.0f, 1.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
		cam.move(vec3(-1.0f, 0.0f, 0.0f));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
		cam.move(vec3(1.0f, 0.0f, 0.0f));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
		cam.move(vec3(0.0f, 0.0f, -1.0f));

	// *****************
	// Update the camera
	// *****************
	cam.update(delta_time);


	// *****************
	// Update cursor pos
	// *****************
	glfwGetCursorPos(window, &current_x, &current_y);

    

    // *******************************************************************
    // Set skybox position to camera position (camera in centre of skybox)
    // *******************************************************************
	skybox.get_transform().position = (cam.get_position());
	static int keystate;
	int newkeystate = glfwGetKey(renderer::get_window(), 'N');
	if (newkeystate && newkeystate != keystate) {
		showGUI = !showGUI;
	}
	keystate = newkeystate;

	if (showGUI) {
		UpdateUI();
	}

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
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

    // *******************
    // Set cubemap uniform
    // *******************
	glUniform1i(eff.get_uniform_location("cubemap"), 0);

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
    // Render mesh
    renderer::render(sphere);

	
	if (showGUI) {
		//glUseProgram(0);
		DrawUI();
	}



    return true;

}

void main()
{
    // Create application
    app application;
    // Set methods
    application.set_load_content(load_content);
	application.set_initialise(initialise);
    application.set_update(update);
    application.set_render(render);
    // Run application
    application.run();
}