#include <graphics_framework.h>

using namespace std;
using namespace std::chrono;
using namespace glm;
using namespace graphics_framework;

// Maximum number of particles
const unsigned int MAX_PARTICLES = 1000;

// A particle
struct particle
{
    vec3 position = vec3(0, 0, 0);
    vec3 velocity = vec3(0, 0, 0);
};

// Particles in the system
particle particles[MAX_PARTICLES];

// The transform feedback buffers
GLuint transform_feedbacks[2];
// The particle buffers
GLuint particle_buffers[2];

// Effects
effect eff;
effect particle_eff;

// Texture
texture tex;

// Current buffer to perform the physics update to
unsigned int front_buf = 0;
// Buffer to render
unsigned int back_buf = 1;

arc_ball_camera cam;
double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise()
{
    glPointSize(5.0f);
    glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);

    return true;
}

bool load_content()
{
    default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    uniform_real_distribution<float> dist;

    tex = texture("..\\resources\\textures\\smoke.png");

    // ******************************************
    // Generate transform feedback buffers on GPU
    // ******************************************
   

    // ********************************
    // Generate particle buffers on GPU
    // ********************************
    

    // *******************
    // Initilise particles
    // *******************
    for (unsigned int i = 0; i < MAX_PARTICLES; ++i)
    {
        
    }

    // ************************
    // Allocate the two buffers
    // ************************
    for (unsigned int i = 0; i < 2; ++i)
    {
        
    }

    // Build effects
    eff.add_shader("smoke.vert", GL_VERTEX_SHADER);
    eff.add_shader("smoke.geom", GL_GEOMETRY_SHADER);
    eff.add_shader("smoke.frag", GL_FRAGMENT_SHADER);
    eff.build();

    particle_eff.add_shader("particle.vert", GL_VERTEX_SHADER);
    particle_eff.add_shader("particle.geom", GL_GEOMETRY_SHADER);
    particle_eff.add_shader("particle.frag", GL_FRAGMENT_SHADER);
    particle_eff.build();

    // Use the particle effect
    renderer::bind(particle_eff);

    // **************************************
    // Tell OpenGL what the output looks like
    // **************************************
    

    // **************
    // Relink program
    // **************
    

    // Set camera properties
    cam.set_position(vec3(0.0f, 00.0f, 10.0f));
    cam.set_target(vec3(0.0f, 0.0f, 0.0f));
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
    return true;
}

bool update(float delta_time)
{
    static bool first_frame = true;

    // ********************
    // Bind particle effect
    // ********************
    

    // **************************
    // Set the delta_time uniform
    // **************************
    

    // *******************************
    // Tell OpenGL we aren't rendering
    // *******************************
    

    // ************************
    // Bind the buffers for use
    // ************************
    

    // ********************************************
    // Define how our data looks like to the shader
    // ********************************************
    

    // ******************************
    // Perform the transform feedback
    // ******************************
    

    // **************************
    // End the transform feedback
    // **************************
    

    // ***********************************
    // Disable the vertex attribute arrays
    // ***********************************
    

    // *************************
    // Switch on rendering again
    // *************************
    

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
    cam.rotate(delta_y, delta_x);

    // Use UP and DOWN to change camera distance
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
        cam.move(-5.0f * delta_time);
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
        cam.move(5.0f * delta_time);

    // Update the camera
    cam.update(delta_time);

    // Update cursor pos
    cursor_x = current_x;
    cursor_y = current_y;

    return true;
}

bool render()
{
    // Bind the effect
    renderer::bind(eff);
    // Set the MVP matrix
    auto M = mat4(1.0f);
    auto V = cam.get_view();
    auto P = cam.get_projection();
    auto MV = V * M;
    glUniformMatrix4fv(
        eff.get_uniform_location("MV"),
        1,
        GL_FALSE,
        value_ptr(MV));
    glUniformMatrix4fv(
        eff.get_uniform_location("P"),
        1,
        GL_FALSE,
        value_ptr(P));
    // Set particle size
    glUniform1f(eff.get_uniform_location("point_size"), 0.1f);

    renderer::bind(tex, 0);
    glUniform1i(eff.get_uniform_location("tex"), 0);

    // This ensures that the particles blend together
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // *******************************************
    // Bind the back particle buffer for rendering
    // *******************************************
    

    // ******************************************************
    // Describe the data we are interested in (just position)
    // ******************************************************
    

    // ****************************************************
    // Perform the render by drawing the transform feedback
    // ****************************************************
    

    // ******************************
    // Disable vertex attribute array
    // ******************************
    

    // Switch off the blending
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    // ***************************
    // Swap front and back buffers
    // ***************************
    

    return true;
}

void main()
{
    // Create application
    app application;
    // Set initialise
    application.set_initialise(initialise);
    // Set load content
    application.set_load_content(load_content);
    // Set update
    application.set_update(update);
    // Set render function
    application.set_render(render);
    // Run application
    application.run();
}