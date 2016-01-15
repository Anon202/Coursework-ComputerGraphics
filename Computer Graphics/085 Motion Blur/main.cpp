#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
effect motion_blur;
effect tex_eff;
texture tex;
directional_light light;
frame_buffer frames[2];
frame_buffer temp_frame;
unsigned int current_frame = 0;
geometry screen_quad;
chase_camera cam;
double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise()
{
    // Set input mode - hide the cursor
    glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Capture initial mouse position
    glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);

    return true;
}

bool load_content()
{
    // **************************************************
    // Create frame buffers - use screen width and height
    // **************************************************
    

    // ******************
    // Create screen quad
    // - positions
    // - tex coords
    // ******************
    

    // Create plane mesh
    meshes["plane"] = mesh(geometry_builder::create_plane());

    // Create scene
    meshes["box"] = mesh(geometry_builder::create_box());
    meshes["tetra"] = mesh(geometry_builder::create_tetrahedron());
    meshes["pyramid"] = mesh(geometry_builder::create_pyramid());
    meshes["disk"] = mesh(geometry_builder::create_disk(20));
    meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20));
    meshes["sphere"] = mesh(geometry_builder::create_sphere(20, 20));
    meshes["torus"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 5.0f));

    // Transform objects
    meshes["box"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
    meshes["box"].get_transform().translate(vec3(-10.0f, 2.5f, -30.0f));
    meshes["tetra"].get_transform().scale = vec3(4.0f, 4.0f, 4.0f);
    meshes["tetra"].get_transform().translate(vec3(-30.0f, 10.0f, -10.0f));
    meshes["pyramid"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
    meshes["pyramid"].get_transform().translate(vec3(-10.0f, 7.5f, -30.0f));
    meshes["disk"].get_transform().scale = vec3(3.0f, 1.0f, 3.0f);
    meshes["disk"].get_transform().translate(vec3(-10.0f, 11.5f, -30.0f));
    meshes["disk"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
    meshes["cylinder"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
    meshes["cylinder"].get_transform().translate(vec3(-25.0f, 2.5f, -25.0f));
    meshes["sphere"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
    meshes["sphere"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
    meshes["torus"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
    meshes["torus"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));

    // Create mesh to chase
    meshes["chaser"] = mesh(geometry_builder::create_box());
    meshes["chaser"].get_transform().position = vec3(0.0f, 0.5f, 0.0f);

    // Set materials
    // Red box
    meshes["box"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    meshes["box"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    meshes["box"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    meshes["box"].get_material().set_shininess(25.0f);
    // Green tetra
    meshes["tetra"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    meshes["tetra"].get_material().set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
    meshes["tetra"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    meshes["tetra"].get_material().set_shininess(25.0f);
    // Blue pyramid
    meshes["pyramid"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    meshes["pyramid"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
    meshes["pyramid"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    meshes["pyramid"].get_material().set_shininess(25.0f);
    // Yellow disk
    meshes["disk"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    meshes["disk"].get_material().set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
    meshes["disk"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    meshes["disk"].get_material().set_shininess(25.0f);
    // Magenta cylinder
    meshes["cylinder"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    meshes["cylinder"].get_material().set_diffuse(vec4(1.0f, 0.0f, 1.0f, 1.0f));
    meshes["cylinder"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    meshes["cylinder"].get_material().set_shininess(25.0f);
    // Cyan sphere
    meshes["sphere"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    meshes["sphere"].get_material().set_diffuse(vec4(0.0f, 1.0f, 1.0f, 1.0f));
    meshes["sphere"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    meshes["sphere"].get_material().set_shininess(25.0f);
    // White torus
    meshes["torus"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    meshes["torus"].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    meshes["torus"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    meshes["torus"].get_material().set_shininess(25.0f);

    // Load texture
    tex = texture("..\\resources\\textures\\checked.gif");

    // Set lighting values
    light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
    light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light.set_direction(vec3(1.0f, 1.0f, -1.0f));

    // Load in shaders
    eff.add_shader("..\\resources\\shaders\\phong.vert", GL_VERTEX_SHADER);
    eff.add_shader("..\\resources\\shaders\\phong.frag", GL_FRAGMENT_SHADER);

    // **************************
    // Load in motion blur effect
    // **************************
    

    tex_eff.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
    tex_eff.add_shader("..\\resources\\shaders\\simple_texture.frag", GL_FRAGMENT_SHADER);
    // Build effects
    eff.build();
    tex_eff.build();

    // ************************
    // Build motion blur effect
    // ************************

    // Set camera properties
    cam.set_pos_offset(vec3(0.0f, 2.0f, 10.0f));
    cam.set_springiness(0.5f);
    cam.move(meshes["chaser"].get_transform().position, eulerAngles(meshes["chaser"].get_transform().orientation));
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

    return true;
}

bool update(float delta_time)
{
    // **********
    // Flip frame
    // **********
    

    // The target object
    static mesh &target_mesh = meshes["chaser"];

    // The ratio of pixels to rotation - remember the fov
    static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
    static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());

    double current_x;
    double current_y;
    // Get the current cursor position
    glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

    // Calculate delta of cursor positions from last frame
    double delta_x = current_x - cursor_x;
    double delta_y = current_y - cursor_y;

    // Multiply deltas by ratios and delta_time - gets actual change in orientation
    delta_x *= ratio_width;
    delta_y *= ratio_height;

    // Rotate cameras by delta
    // x - delta_y
    // y - delta_x
    // z - 0
    cam.rotate(vec3(delta_y, delta_x, 0.0f));

    // Use keyboard to rotate target_mesh
    // - QE rotate on y-axis
    if (glfwGetKey(renderer::get_window(), 'Q'))
        target_mesh.get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
    if (glfwGetKey(renderer::get_window(), 'E'))
        target_mesh.get_transform().rotate(vec3(0.0f, -half_pi<float>(), 0.0f) * delta_time);

    // Use keyboard to move the target_mesh
    // - WSAD
    if (glfwGetKey(renderer::get_window(), 'W'))
        target_mesh.get_transform().translate(target_mesh.get_transform().orientation * vec3(0.0f, 0.0f, -1.0f) * 5.0f * delta_time);
    if (glfwGetKey(renderer::get_window(), 'S'))
        target_mesh.get_transform().translate(target_mesh.get_transform().orientation * vec3(0.0f, 0.0f, 1.0f) * 5.0f * delta_time);
    if (glfwGetKey(renderer::get_window(), 'A'))
        target_mesh.get_transform().translate(target_mesh.get_transform().orientation * vec3(-1.0f, 0.0f, 0.0f) * 5.0f * delta_time);
    if (glfwGetKey(renderer::get_window(), 'D'))
        target_mesh.get_transform().translate(target_mesh.get_transform().orientation * vec3(1.0f, 0.0f, 0.0f) * 5.0f * delta_time);

    // Move camera - update target position and rotation
    cam.move(target_mesh.get_transform().position, eulerAngles(target_mesh.get_transform().orientation));

    // Update the camera
    cam.update(delta_time);

    // Update cursor pos
    cursor_x = current_x;
    cursor_y = current_y;

    // Rotate the sphere
    meshes["sphere"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);

    return true;
}

bool render()
{
    // !!!!!!!!!!!!!!! FIRST PASS !!!!!!!!!!!!!!!!

    // *******************************
    // Set render target to temp frame
    // *******************************
    

    // ***********
    // Clear frame
    // ***********
    

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
        // Create MV matrix
        auto MV = V * M;
        // Set MV matrix uniform
        glUniformMatrix4fv(
            eff.get_uniform_location("MV"), // Location of uniform
            1, // Number of values - 1 mat4
            GL_FALSE, // Transpose the matrix?
            value_ptr(MV)); // Pointer to matrix data
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
            value_ptr(m.get_transform().get_normal_matrix()));
        // Bind material
        renderer::bind(m.get_material(), "mat");
        // Bind light
        renderer::bind(light, "light");
        // Bind texture
        renderer::bind(tex, 0);
        // Set tex uniform
        glUniform1i(eff.get_uniform_location("tex"), 0);
        // Set eye position
        glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

        // Render mesh
        renderer::render(m);
    }

    // !!!!!!!!!!!!!!! SECOND PASS !!!!!!!!!!!!!!!!

    // **********************************
    // Set render target to current frame
    // **********************************
    

    // ***********
    // Clear frame
    // ***********
    

    // ***********************
    // Bind motion blur effect
    // ***********************
    

    // ******************************
    // MVP is now the identity matrix
    // ******************************
    

    // ***********
    // Bind frames
    // ***********
    

    // ****************
    // Set blend factor
    // ****************
    

    // ******************
    // Render screen quad
    // ******************
    

    // !!!!!!!!!!!!!!! SCREEN PASS !!!!!!!!!!!!!!!!

    // ************************************
    // Set render target back to the screen
    // ************************************
    

    // Use texturing effect
    renderer::bind(tex_eff);

    // **********************
    // Set MVP matrix uniform
    // **********************
    

    // ******************************
    // Bind texture from frame buffer
    // ******************************
    

    // ***************
    // Set the uniform
    // ***************
    

    // **********************
    // Render the screen quad
    // **********************
    

    return true;
}

void main()
{
    // Create application
    app application;
    // Set load content, update and render methods
    application.set_initialise(initialise);
    application.set_load_content(load_content);
    application.set_update(update);
    application.set_render(render);
    // Run application
    application.run();
}