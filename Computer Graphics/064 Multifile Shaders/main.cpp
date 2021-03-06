#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
texture tex;
target_camera cam;
directional_light light;
vector<point_light> points(4);
vector<spot_light> spots(5);

bool load_content()
{
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

    // ***********************
    // Set materials
    // - all emissive is black
    // - all specular is white
    // - all shininess is 25
    // ***********************
    // Red box
    
    // Green tetra
    
    // Blue pyramid
    
    // Yellow disk
    
    // Magenta cylinder
    
    // Cyan sphere
    
    // White torus
    

    // ************
    // Load texture
    // ************
    

    // *******************
    // Set lighting values
    // *******************
    // ambient intensity (0.3, 0.3, 0.3)
    
    // Light colour (0.7, 0.7, 0.7)
    
    // Light direction (1.0, 1.0, -1.0)
    
    // Point 0
    // Position (-25, 5, -15)
    // Red
    // 20 range
    
    // Point 1
    // Position (-25, 5, -35)
    // Red
    // 20 range
    
    // Point 2
    // Position (-10, 5, -15)
    // Red
    // 20 range
    
    // Point 3
    // Position (-10, 5, -35)
    // Red
    // 20 range
    
    // Spot 0
    // Position (-25, 10, -15)
    // Green
    // Direction (1, -1, -1) normalized
    // 20 range
    // 0.5 power
    
    // Spot 1
    // Position (-25, 10, -35)
    // Green
    // Direction (1, -1, 1) normalized
    // 20 range
    // 0.5 power
    
    // Spot 2
    // Position (-10, 10, -15)
    // Green
    // Direction (-1, -1, -1) normalized
    // 20 range
    // 0.5 power
    
    // Spot 3
    // Position (-10, 10, -35)
    // Green
    // Direction (-1, -1, 1) normalized
    // 20 range
    // 0.5 power
    
    // Spot 4
    // Position (-17.5, 15, -25)
    // Blue
    // Direction (0, -1, 0)
    // 30 range
    // 1.0 power
    

    // Load in shaders
    eff.add_shader("shader.vert", GL_VERTEX_SHADER);
    // *********************************
    // Name of fragment shaders required
    // *********************************
    vector<string> frag_shaders
    {
        "shader.frag"
        // Other three shaders
    };
    eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);
    // Build effect
    eff.build();

    // Set camera properties
    cam.set_position(vec3(50.0f, 10.0f, 50.0f));
    cam.set_target(vec3(0.0f, 0.0f, 0.0f));
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
    return true;
}

bool update(float delta_time)
{
    if (glfwGetKey(renderer::get_window(), '1'))
        cam.set_position(vec3(50, 10, 50));
    if (glfwGetKey(renderer::get_window(), '2'))
        cam.set_position(vec3(-50, 10, 50));
    if (glfwGetKey(renderer::get_window(), '3'))
        cam.set_position(vec3(-50, 10, -50));
    if (glfwGetKey(renderer::get_window(), '4'))
        cam.set_position(vec3(50, 10, -50));

    // Rotate the sphere
    meshes["sphere"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);

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
        // ********************
        // Set M matrix uniform
        // ********************
        
        // ***********************
        // Set N matrix uniform
        // - remember - 3x3 matrix
        // ***********************
        
        // *************
        // Bind material
        // *************
        
        // **********************
        // Bind directional light
        // **********************
        
        // *****************
        // Bind point lights
        // *****************
        
        // ****************
        // Bind spot lights
        // ****************
        
        // ************
        // Bind texture
        // ************
        
        // ***************
        // Set tex uniform
        // ***************
        
        // *****************************
        // Set eye position
        // - Get this from active camera
        // *****************************
        

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