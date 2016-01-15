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
        // Face 1
        
        // Face 2
        
        // Face 3
        
        // Face 4
        
        // Face 5
        
        // Face 6
        
    };
    geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
    skybox = mesh(geom);
    // ***********************************
    // Scale box by 10 - allows a distance
    // ***********************************
   

    // ******************************************************
    // Load the cubemap
    // - create array of six filenames +x, -x, +y, -y, +z, -z
    // ******************************************************
    array<string, 6> filenames =
    {
        
    };
    // ***************
    // Create cube_map
    // ***************
    

    // ******************************
    // Load in environment map shader
    // ******************************
    
    // Build effect
    eff.build();

    // *********************
    // Load in skybox effect
    // *********************
    
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
    

    return true;
}

bool render()
{
    // *********************************
    // Disable depth test and depth mask
    // *********************************
    
    // ******************
    // Bind skybox effect
    // ******************
    
    // ****************************
    // Calculate MVP for the skybox
    // ****************************
    auto M = mat4(1.0f); // Change!!!
    auto V = mat4(1.0f); // Change!!!
    auto P = mat4(1.0f); // Change!!!
    auto MVP = mat4(1.0f); // Change!!!

    // **********************
    // Set MVP matrix uniform
    // **********************
    
    // *******************
    // Set cubemap uniform
    // *******************
    
    // *************
    // Render skybox
    // *************
    
    // ********************************
    // Enable depth test and depth mask
    // ********************************
    

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