#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
material mat;
effect main_eff;
effect shadow_eff;
texture tex;
free_camera cam;
spot_light spot;
shadow_map shadow;


// initialise params
GLFWwindow* window;
double xpos = 0;
double ypos = 0;

double current_x = 0;
double current_y = 0;

double new_x = 0;
double new_y = 0;

bool firstMouse = true;

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


	return true;
}

bool load_content()
{
    // *****************
    // Create shadow map
    // - use screen size
    // *****************
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());

    // Create plane mesh
    meshes["plane"] = mesh(geometry_builder::create_plane());

    // *********************************************
    // Create "teapot" mesh by loading in teapot.3ds
    // *********************************************
	meshes["teapot"] = mesh(geometry("..\\resources\\models\\teapot.3ds"));

    // ***********************************************
    // Need to rotate the teapot on x by negative pi/2
    // ***********************************************
	float theta = -pi<float>() / 2;

	meshes["teapot"].get_transform().rotate(angleAxis(theta, vec3(1.0, 0.0, 0.0)));

    // *****************
    // Scale the teapot
    // - (0.1, 0.1, 0.1)
    // *****************
	meshes["teapot"].get_transform().scale = vec3(0.1, 0.1, 0.1);

    // ************
    // Load texture
    // ************
	tex = texture("..\\resources\\textures\\checked.gif");

    // ***********************
    // Set materials
    // - all emissive is black
    // - all specular is white
    // - all shininess is 25
    // ***********************
    // White plane

    // White teapot
	mat.set_emissive(vec4(0.0, 0.0, 0.0, 1.0));
	mat.set_shininess(25.0f);
	mat.set_specular(vec4(1.0, 1.0, 1.0, 1.0));
	mat.set_diffuse(vec4(1.0, 1.0, 1.0, 1.0));

	meshes["teapot"].set_material(mat);
	meshes["plane"].set_material(mat);
    // *******************
    // Set spot properties
    // *******************
    // Pos (20, 30, 0)
    // White
    // Direction (-1, -1, 0) normalized
    // 50 range
    // 10 power
	spot.set_position(vec3(20, 30, 0));
	spot.set_direction(normalize(vec3(-1, -1, 0)));
	spot.set_light_colour(vec4(1.0, 1.0, 1.0, 1.0));
	spot.set_range(50);
	spot.set_power(10);

    // Load in shaders
    main_eff.add_shader("..\\resources\\shaders\\shadow.vert", GL_VERTEX_SHADER);
    vector<string> frag_shaders
    {
        "shader.frag",
        "..\\resources\\shaders\\parts\\spot.frag",
        "..\\resources\\shaders\\parts\\shadow.frag"
    };
    main_eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

    shadow_eff.add_shader("..\\resources\\shaders\\spot.vert", GL_VERTEX_SHADER);
    shadow_eff.add_shader("..\\resources\\shaders\\spot.frag", GL_FRAGMENT_SHADER);

    // Build effects
    main_eff.build();
    shadow_eff.build();

    // Set camera properties
    cam.set_position(vec3(0.0f, 50.0f, -75.0f));
    cam.set_target(vec3(0.0f, 0.0f, 0.0f));
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 10000.0f);
    return true;
}

bool update(float delta_time)
{
    // Rotate the teapot
    meshes["teapot"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()) * delta_time);

    // ****************************************************
    // Update the shadow map properties from the spot light
    // ****************************************************

	shadow.light_position = spot.get_position();
	shadow.light_dir = spot.get_direction();

    // Press s to save
    if (glfwGetKey(renderer::get_window(), 'S') == GLFW_PRESS)
        shadow.buffer->save("test.png");

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
	/*if (firstMouse)
	{
	current_x = xpos;
	current_y = ypos;
	firstMouse = false;
	}*/

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
		cam.move(vec3(0.0f, 0.0f, 1.0f));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
		cam.move(vec3(-1.0f, 0.0f, 0.0f));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
		cam.move(vec3(1.0f, 0.0f, 0.0f));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
		cam.move(vec3(0.0f, 0.0f, -1.0f));


	// ***********
	// Move camera
	// ***********

	// *****************
	// Update the camera
	// *****************
	cam.update(delta_time);


	// *****************
	// Update cursor pos
	// *****************
	glfwGetCursorPos(window, &current_x, &current_y);


	return true;
}

bool render()
{
    // *******************************
    // Set render target to shadow map
    // *******************************

	renderer::set_render_target(shadow);

	// **********************
	// Clear depth buffer bit
	// **********************
	glClear(GL_DEPTH_BUFFER_BIT);

	// ****************************
	// Set render mode to cull face
	// ****************************
	glCullFace(GL_FRONT);

    // Bind shader
    renderer::bind(shadow_eff);

    // Render meshes
    for (auto &e : meshes)
    {
        auto m = e.second;
        // Create MVP matrix
        auto M = m.get_transform().get_transform_matrix();
        // *********************************
        // View matrix taken from shadow map
        // *********************************
		auto V = shadow.get_view(); 

        auto P = cam.get_projection();
        auto MVP = P * V * M;
        // Set MVP matrix uniform
        glUniformMatrix4fv(
            shadow_eff.get_uniform_location("MVP"), // Location of uniform
            1, // Number of values - 1 mat4
            GL_FALSE, // Transpose the matrix?
            value_ptr(MVP)); // Pointer to matrix data
        // Render mesh
        renderer::render(m);
    }

    // ************************************
    // Set render target back to the screen
    // ************************************
	renderer::set_render_target();

    // *********************
    // Set cull face to back
    // *********************
	glCullFace(GL_BACK);

    // Bind shader
    renderer::bind(main_eff);

    // Render meshes
    for (auto &e : meshes)
    {
        auto m = e.second;
        // Create MVP matrix
        auto M = m.get_transform().get_transform_matrix();
        auto V = cam.get_view();
        auto P = cam.get_projection();
        auto MVP = P * V * M;
        // Set MVP matrix uniform
        glUniformMatrix4fv(
            main_eff.get_uniform_location("MVP"), // Location of uniform
            1, // Number of values - 1 mat4
            GL_FALSE, // Transpose the matrix?
            value_ptr(MVP)); // Pointer to matrix data
        // Set M matrix uniform
        glUniformMatrix4fv(
            main_eff.get_uniform_location("M"),
            1,
            GL_FALSE,
            value_ptr(M));
        // Set N matrix uniform
        glUniformMatrix3fv(
            main_eff.get_uniform_location("N"),
            1,
            GL_FALSE,
            value_ptr(m.get_transform().get_normal_matrix()));
        // *******************
        // Set light transform
        // *******************
		

		
		auto lV = shadow.get_view();

		auto lMVP = P * lV * M;

		//mat4 rot = rotate

		glUniformMatrix4fv(
			main_eff.get_uniform_location("lightMVP"),
			1,
			GL_FALSE,
			value_ptr(lMVP));

        // Bind material
        renderer::bind(m.get_material(), "mat");
        // Bind spot lights
        renderer::bind(spot, "spot");
        // Bind texture
        renderer::bind(tex, 0);
        // Set tex uniform
        glUniform1i(main_eff.get_uniform_location("tex"), 0);
        // Set eye position
        glUniform3fv(main_eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

        // ***********************
        // Bind shadow map texture
        // - use texture unit 1
        // ***********************
		renderer::bind(shadow.buffer->get_depth(), 1);
		glUniform1i(main_eff.get_uniform_location("shadow_map"), 1);

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
	application.set_initialise(initialise);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}