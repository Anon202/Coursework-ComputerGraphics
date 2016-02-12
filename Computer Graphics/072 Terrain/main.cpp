#include <graphics_framework.h>
#include <glm/glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh terr;
effect eff;
target_camera cam;

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
	auto data = new vec4[width * depth];
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

			point.y = data[z * height_map.get_width() + x].y * height_scale;
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
			indices.push_back(top_right);
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
			w.x = clamp(((1.0 - length(data[height_map.get_width() * z + x].y) ) / 0.25), 0.0, 1.0);
			w.y = clamp(((1.0 - length(data[height_map.get_width() * z + x].y - 0.15)) / 0.25), 0.0, 1.0);
			w.z = clamp(((1.0 - length(data[height_map.get_width() * z + x].y - 0.5)) / 0.25), 0.0, 1.0);
			w.w = clamp(((1.0 - length(data[height_map.get_width() * z + x].y - 0.9)) / 0.25), 0.0, 1.0);

            // ********************************
            // Sum the components of the vector
            // ********************************
            

            // ********************
            // Divide weight by sum
            // ********************
            

            // *************************
            // Add tex weight to weights
            // *************************
            
        }
    }

    // *************************************
    // Add necessary buffers to the geometry
    // *************************************
    

    // ***********
    // Delete data
    // ***********
    
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

    // ************************
    // Load in necessary effect
    // - Part 1 colour
    // - Part 2 lighting
    // - Part 3 textured
    // - Part 4 texture weighted
    // *************************
    eff.add_shader("..\\resources\\shaders\\colour.vert", GL_VERTEX_SHADER); 
    eff.add_shader("..\\resources\\shaders\\colour.frag", GL_FRAGMENT_SHADER);

    // Build effect
    eff.build();

    // *************************************
    // Set any othe necessary content values
    // *************************************


    // Set camera properties
    cam.set_position(vec3(0.0f, 5.0f, 10.0f));
    cam.set_target(vec3(0.0f, 0.0f, 0.0f));
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
    return true;
}

bool update(float delta_time)
{
    cam.update(delta_time);

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

    // ****************************
    // Set other necessary uniforms
    // ****************************


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
    application.set_update(update);
    application.set_render(render);
    // Run application
    application.run();
}