#include "main.h"

mesh skybox;
effect eff;
effect sky_eff;
effect water_eff;

effect terr_eff;
cubemap cube_map;

map<string, mesh> meshes;
map<string, material> materials;

SceneManager* myScene;  // pointer to a scene manager!

bool initialise()
{
	double xpos = 0; // create initial vars for mouse position
	double ypos = 0;

	
	// ********************************
	// Set input mode - hide the cursor
	// ********************************
	GLFWwindow* window = renderer::get_window();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ******************************
	// Capture initial mouse position
	// ******************************
	
	glfwGetCursorPos(window, &xpos, &ypos);

	myScene = new SceneManager(xpos, ypos); // pass in (copied in constructor)



	// initialise the cameras and store in pointer list

	// static target camera at pos [0]
	myScene->cam = new target_camera();
	myScene->cameraList.push_back(myScene->cam);  // add to list so as to not loose the pointer to the camera

	// create target camera
	myScene->cam->set_position(vec3(50.0f, 10.0f, 50.0f));
	myScene->cam->set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	myScene->cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);


	// free_camera!
	myScene->cam = new free_camera();
	myScene->cameraList.push_back(myScene->cam); // add to list (so can be deleted at end)


	// Set camera properties for free camera (default)
	myScene->cam->set_position(vec3(50.0f, 10.0f, 50.0f));
	myScene->cam->set_target(vec3(0.0f, 0.0f, 0.0f));
	myScene->cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	myScene->Create();  // run create method for scene man
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

	////// GL POLYGON MODE

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
			w.x = clamp((1.0 - (length(data[height_map.get_width() * z + x].y) / 0.25)), 0.0, 1.0);
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

	directional_light* light = myScene->light;  // create local pointer to the scenes light
	

	// CREATE TERRAIN
	geometry terrGeom; // geom to load into
	
	// Load height map
	texture height_map("..\\resources\\textures\\heightmaps\\myHeightMap.png");

	// Generate terrain
	generate_terrain(terrGeom, height_map, 20, 20, 2.0f);

	// create terrain object

	// Use geometry to create terrain mesh
	meshes["terr"] = mesh(terrGeom);
	
	vector<texture*> terrTextList;				// local list of textures

	terrTextList.push_back( new texture("..\\resources\\textures\\sand.dds"));
	terrTextList.push_back( new texture("..\\resources\\textures\\grass.dds"));
	terrTextList.push_back( new texture("..\\resources\\textures\\rock.dds"));
	terrTextList.push_back( new texture("..\\resources\\textures\\snow.dds"));

	myScene->texList.push_back(terrTextList);

	// Create plane mesh
	meshes["plane"] = mesh(geometry_builder::create_plane(200,200));

	// Create scene
	meshes["box"] = mesh(geometry_builder::create_box());
	meshes["pyramid"] = mesh(geometry_builder::create_pyramid());

	// Red box
	materials["box"].set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));

	// Blue pyramid
	materials["pyramid"].set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));



	for (auto &e : materials)
	{
		e.second.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
		e.second.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		e.second.set_shininess(2.0f);
	}

	
	terr_eff.add_shader("shader.vert", GL_VERTEX_SHADER);
	terr_eff.add_shader("shader.frag", GL_FRAGMENT_SHADER);
	terr_eff.add_shader("..\\resources\\shaders\\parts\\weighted_texture.frag", GL_FRAGMENT_SHADER);
	// Build effect
	terr_eff.build();


	myScene->plane = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(10.0f, 10.0f, 10.0f), &meshes["terr"], &materials["plane"], terrTextList, &terr_eff, light, terrn);

	vector<texture*> objTextList;
	objTextList.push_back(new texture("..\\resources\\textures\\checked.gif"));

	myScene->texList.push_back(objTextList);


	vector<texture*> waterText;
	waterText.push_back(new texture("..\\resources\\textures\\checked.gif"));

	myScene->texList.push_back(waterText);

	//root = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(10.0f, 10.0f, 10.0f), &meshes["plane"], &materials["plane"], &tex, &eff, P, V, eyeP, &light);

	Obj *water = new Obj(vec3(0.0f, 0.3f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(0.1f, 0.1f, 0.1f), &meshes["plane"], &materials["plane"], waterText, &water_eff, light, waterObj);

	Obj *box = new Obj(vec3(-10.0f, 2.5f, -30.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(0.5f, 0.5f, 0.5f), &meshes["box"], &materials["box"], objTextList, &eff, light, object);

	Obj *pyra = new Obj(vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &meshes["pyramid"], &materials["pyramid"], objTextList, &eff, light, object);

	myScene->plane->addChild(box, "box");

	myScene->plane->addChild(water, "water");

	box->addChild(pyra, "pyramid");
	myScene->list.push_back(water);
	myScene->list.push_back(myScene->plane);
	myScene->list.push_back(box);
	myScene->list.push_back(pyra);

	water_eff.add_shader("..\\resources\\shaders\\phong2.vert", GL_VERTEX_SHADER);
	water_eff.add_shader("..\\resources\\shaders\\water.frag", GL_FRAGMENT_SHADER);
	water_eff.build();


	// Load in shaders
	eff.add_shader("..\\resources\\shaders\\phong.vert", GL_VERTEX_SHADER);
	eff.add_shader("..\\resources\\shaders\\phong.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();


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
		vec3(1.0, -1.0, 1.0),	// 3
		vec3(-1.0, -1.0, 1.0),	// 2
		vec3(-1.0, 1.0, 1.0),	//1
		vec3(1.0, 1.0, 1.0),    // 4
    };

    geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);  // add position to buffer
	skybox = mesh(geom);

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

	
    // *********************
    // Load in skybox effect
    // *********************
	sky_eff.add_shader("..\\resources\\shaders\\skybox.vert", GL_VERTEX_SHADER);
	sky_eff.add_shader("..\\resources\\shaders\\skybox.frag", GL_FRAGMENT_SHADER);

    // Build effect
    sky_eff.build();

	myScene->root = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(100.0f, 100.0f, 100.0f), &skybox, &materials["skybox"], objTextList, &sky_eff, light, sky);
	myScene->list.push_back(myScene->root);

	//myScene->plane->addChild(myScene->root, "root");


	// plane geometry not working
	//root->addChild(plane, "plane");
	//list.push_back(plane);

    return true;
}

bool update(float delta_time)
{
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_T))    // need to get an enum for camera tyoe
		myScene->cam = myScene->cameraList[0];
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F))
		myScene->cam = myScene->cameraList[1];

	free_camera* freeCam = NULL;
	freeCam = dynamic_cast<free_camera*>(myScene->cam);

	
	

	if (freeCam)
	{

		GLFWwindow* window = renderer::get_window();

		// The ratio of pixels to rotation - remember the fov
		static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
		static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());

		double new_x = 0;
		double new_y = 0;

		glfwGetCursorPos(window, &new_x, &new_y);	// Get the current cursor position

		if (myScene->firstMouse)							 // if first mouse take cursor positons from initalised vars
		{
			myScene->current_x = myScene->initialX;
			myScene->current_y = myScene->initialY;
			myScene->firstMouse = false;
		}

		double delta_x = new_x - myScene->current_x;		 // Calculate delta of cursor positions from last frame
		double delta_y = new_y - myScene->current_y;

		delta_x *= ratio_width;					 // Multiply deltas by ratios - gets actual change in orientation
		delta_y *= -ratio_height;

		freeCam->rotate((float)delta_x, (float)delta_y);  // Rotate cameras by delta :: delta_y - x-axis rotation :: delta_x - y-axis rotation



		if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
			freeCam->move(vec3(0.0f, 0.0f, 1.0f));
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
			freeCam->move(vec3(-1.0f, 0.0f, 0.0f));
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
			freeCam->move(vec3(1.0f, 0.0f, 0.0f));
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
			freeCam->move(vec3(0.0f, 0.0f, -1.0f));


		glfwGetCursorPos(window, &myScene->current_x, &myScene->current_y);  // update cursor pos
	}

	myScene->cam->update(delta_time);  // update the camera
	
	myScene->root->update(myScene->root, mat4(1));

	myScene->plane->update(myScene->plane, mat4(1));
    return true;
}

bool render()
{

	myScene->root->render(myScene->root);  // is sky true (enable/disable depth)

	myScene->plane->render(myScene->plane); // check -p;lane

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

	for (uint i = 0; i < myScene->list.size(); ++i)          //// getting a memory leak without this regardless of release method...
		delete myScene->list[i];

	myScene->list.clear();

	myScene->Release(); // method to free memory and delete pointers
	delete myScene;
	myScene = NULL;
}