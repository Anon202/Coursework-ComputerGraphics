#include <graphics_framework.h>
#include <glm/glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh terr;
effect eff;
free_camera cam;

directional_light light;

texture tex[4];



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


void generate_terrain(geometry &geom, const texture &height_map, unsigned int width, unsigned int depth, float height_scale)
{
    // Contains our position data
    vector<vec3> positions;
    // Contains our normal data
    vector<vec3> normals;
    // Contains our texture coordinate data
    vector<vec2> tex_coords;
    // Contains our texture weights
    vector<vec4> tex_weights;
    // Contains our index data
    vector<unsigned int> indices;

    // ***************************************
    // Extract the texture data from the image
    // ***************************************
	glBindTexture(GL_TEXTURE_2D, height_map.get_id());
	auto data = new vec4[height_map.get_width() * height_map.get_height()];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (void*)data);
    

    // Determine ratio of height map to geometry
    float width_point = static_cast<float>(width) / static_cast<float>(height_map.get_width());
    float depth_point = static_cast<float>(depth) / static_cast<float>(height_map.get_height());

    // Point to work on
    vec3 point;

    // ***********************************************************************
    // Part 1 - Iterate through each point, calculate vertex and add to vector
    // ***********************************************************************
    for (int x = 0; x < height_map.get_width(); ++x)
    {
        // *****************************
        // Calculate x position of point
        // *****************************
		float w2 = width / 2;
		point.x = -w2 + (width_point * x);

        for (int z = 0; z < height_map.get_height(); ++z)
        {
            // *****************************
            // Calculate z position of point
            // *****************************
			float d2 = depth / 2;
			point.z = -d2 + (depth_point*z);

            // ****************************************************
            // Y position based on red component of height map data
            // ****************************************************

			point.y = data[(z * height_map.get_width()) + x].y * height_scale;
            // **************************
            // Add point to position data
            // **************************
			positions.push_back(point);
        }
    }

    // ***********************
    // Part 1 - Add index data
    // ***********************
    for (unsigned int x = 0; x < height_map.get_width() - 1; ++x)
    {
        for (unsigned int y = 0; y < height_map.get_height() - 1; ++y)
        {
            // *************************
            // Get four corners of patch
            // *************************
			int top_left = (y * height_map.get_width()) + x;
			int top_right = (y * height_map.get_width()) + x + 1;
			int bottom_left = ((y + 1)*height_map.get_width()) + x;
			int bottom_right = ((y + 1)*height_map.get_height()) + x + 1;

            // ********************************
            // Push back indices for triangle 1
            // ********************************
			indices.push_back(top_left);
			indices.push_back(bottom_right);
			indices.push_back(bottom_left);
            
            // ********************************
            // Push back indices for triangle 2
            // ********************************
			indices.push_back(top_left);
			indices.push_back(top_right);
			indices.push_back(bottom_right);
            
        }
    }

    // Resize the normals buffer
    normals.resize(positions.size());

    // *********************************************
    // Part 2 - Calculate normals for the height map
    // *********************************************
    for (unsigned int i = 0; i < indices.size() / 3; ++i)
    {
        // ****************************
        // Get indices for the triangle
        // ****************************
		int idx1 = indices[i * 3];
		int idx2 = indices[i * 3 + 1];
		int idx3 = indices[i * 3 + 2];

        // ***********************************
        // Calculate two sides of the triangle
        // ***********************************
		vec3 side1 = positions[idx1] - positions[idx3];
		vec3 side2 = positions[idx1] - positions[idx2];
        

        // ******************************************
        // Normal is cross product of these two sides
        // ******************************************
		vec3 n = cross(side2, side1);       

        // **********************************************************************
        // Add to normals in the normal buffer using the indices for the triangle
        // **********************************************************************
		normals[idx1] += n;
		normals[idx2] += n;
		normals[idx3] += n;
        
    }

    // *************************
    // Part 2 - Normalize all the normals
    // *************************
	for (auto &n : normals)
	{
		n = normalize(n);
	}

    // *********************************************
    // Part 3 - Add texture coordinates for geometry
    // *********************************************
    for (unsigned int x = 0; x < height_map.get_width(); ++x)
    {
        for (unsigned int z = 0; z < height_map.get_height(); ++z)
        {
			vec2 v;
			v.x = width_point * x;
			v.y = depth_point * z;
			tex_coords.push_back(v);
        }
    }

    // **************************************************
    // Part 4 - Calculate texture weights for each vertex
    // **************************************************
    for (unsigned int x = 0; x < height_map.get_width(); ++x)
    {
        for (unsigned int z = 0; z < height_map.get_height(); ++z)
        {
            // ********************
            // Calculate tex weight
            // ********************
			vec4 w;
			w.x = clamp((1.0 - (length(data[height_map.get_width() * z + x].y)  / 0.25)), 0.0, 1.0);
			w.y = clamp((1.0 - (length(data[height_map.get_width() * z + x].y - 0.15) / 0.25)), 0.0, 1.0);
			w.z = clamp((1.0 - (length(data[height_map.get_width() * z + x].y - 0.5) / 0.25)), 0.0, 1.0);
			w.w = clamp((1.0 - (length(data[height_map.get_width() * z + x].y - 0.9) / 0.25)), 0.0, 1.0);

            // ********************************
            // Sum the components of the vector
            // ********************************
			float total = w.x + w.y + w.z + w.w;

            // ********************
            // Divide weight by sum
            // ********************
			w = w / total;

            // *************************
            // Add tex weight to weights
            // *************************
			tex_weights.push_back(w);
		}
    }

    // *************************************
    // Add necessary buffers to the geometry
    // *************************************
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_index_buffer(indices);
	geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	geom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);
	
    // ***********
    // Delete data
    // ***********
	delete data;
}

bool load_content()
{
    // Geometry to load into
    geometry geom;

    // Load height map
    texture height_map("..\\resources\\textures\\heightmaps\\terrain.png");

    // Generate terrain
    generate_terrain(geom, height_map, 20, 20, 2.0f);

    // Use geometry to create terrain mesh
    terr = mesh(geom);


	tex[0] = texture("..\\resources\\textures\\sand.dds");
	tex[1] = texture("..\\resources\\textures\\grass.dds");
	tex[2] = texture("..\\resources\\textures\\rock.dds");
	tex[3] = texture("..\\resources\\textures\\snow.dds");

    // ************************
    // Load in necessary effect
    // - Part 1 colour
    // - Part 2 lighting
    // - Part 3 textured
    // - Part 4 texture weighted
    // *************************
	//eff.add_shader("..\\resources\\shaders\\colour.vert", GL_VERTEX_SHADER); 
    //eff.add_shader("..\\resources\\shaders\\colour.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("terrain.vert", GL_VERTEX_SHADER);
	eff.add_shader("terrain.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("..\\resources\\shaders\\parts\\weighted_texture.frag", GL_FRAGMENT_SHADER);

	

    // Build effect
    eff.build();

    // *************************************
    // Set any othe necessary content values
    // *************************************

	// maybe
	material mat;
	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_diffuse(vec4(0.53, 0.45, 0.37, 1.0));
	mat.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mat.set_shininess(25.0f);
	terr.set_material(mat);

	// ambient intensity (0.3, 0.3, 0.3)
	light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));

	// Light colour white
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Light direction (1.0, 1.0, -1.0)
	light.set_direction(vec3(1.0f, 1.0f, -1.0f));


    // Set camera properties
    cam.set_position(vec3(0.0f, 5.0f, 10.0f));
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



    return true;
}

bool render()
{
    // Bind effect
    renderer::bind(eff);
    // Create MVP matrix
    auto M = terr.get_transform().get_transform_matrix();
    auto V = cam.get_view();
    auto P = cam.get_projection();
    auto MVP = P * V * M;
    // Set MVP matrix uniform
    glUniformMatrix4fv(
        eff.get_uniform_location("MVP"),
        1,
        GL_FALSE,
        value_ptr(MVP));
    // Set colour uniform
    glUniform4fv(eff.get_uniform_location("colour"), 1, value_ptr(vec4(0.7f, 0.7f, 0.7f, 1.0f)));

	glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

	glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(terr.get_transform().get_normal_matrix()));

    // ****************************
    // Set other necessary uniforms
    // ****************************
	

	// *************
	// Bind material
	// *************
	renderer::bind(terr.get_material(), "mat");


	// **********
	// Bind light
	// **********
	renderer::bind(light, "light");

	// ************
	// Bind texture
	// ************
	renderer::bind(tex[0], 0);
	renderer::bind(tex[1], 1);
	renderer::bind(tex[2], 2);
	renderer::bind(tex[3], 3);

	

	glUniform1i(eff.get_uniform_location("tex[0]"), 0);
	glUniform1i(eff.get_uniform_location("tex[1]"), 1);
	glUniform1i(eff.get_uniform_location("tex[2]"), 2);
	glUniform1i(eff.get_uniform_location("tex[3]"), 3);


    // Render terrain
    renderer::render(terr);

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