#include "main.h"
#include "Obj.h"


// will not need when fin
mesh sphere;
mesh skybox;
effect eff;
effect sky_eff;
cubemap cube_map;

texture tex;
// globals needed camera list object list cam and root pointer/// window pointer.  -- be good to use GLFWwindow* window = renderer::get_window(); so only 1 window? maybe

camera* cam = NULL;

vector<camera*> cameraList;

Obj* root = NULL;
vector<Obj*> list;


map<string, mesh> meshes;
map<string, material> materials;

Obj *ball = NULL;
// initialise params
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
	GLFWwindow* window = renderer::get_window();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ******************************
	// Capture initial mouse position
	// ******************************
	glfwGetCursorPos(window, &xpos, &ypos);


	// initialise the cameras and store in pointer list

	// static target camera at pos [0]
	cam = new target_camera();
	cameraList.push_back(cam);  // add to list so as to not loose the pointer to the camera

	// create target camera
	cam->set_position(vec3(50.0f, 10.0f, 50.0f));
	cam->set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);


	// free_camera!
	cam = new free_camera();
	cameraList.push_back(cam); // add to list (so can be deleted at end)


	// Set camera properties for free camera (default)
	cam->set_position(vec3(50.0f, 10.0f, 50.0f));
	cam->set_target(vec3(0.0f, 0.0f, 0.0f));
	cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);


	return true;
}

bool load_content()
{

	directional_light light;


	// Create plane mesh
	meshes["plane"] = mesh(geometry_builder::create_plane());

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


	// **************************
	// Load texture - checked.gif
	// **************************
	tex = texture("..\\resources\\textures\\checked.gif");


	// *******************
	// Set lighting values
	// *******************
	// ambient intensity (0.3, 0.3, 0.3)
	light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));

	// Light colour white
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Light direction (1.0, 1.0, -1.0)
	light.set_direction(vec3(1.0f, 1.0f, -1.0f));

	/*
	mat4 P = cam->get_projection();
	mat4 V = cam->get_view();
	mat4 PV = P * V;

	vec3 eyeP = cam->get_position();*/





	//meshes["pyramid"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	//meshes["pyramid"].get_transform().translate(vec3(-10.0f, 7.5f, -30.0f));
	//tetra = Obj()



	// Load in shaders
	eff.add_shader("..\\resources\\shaders\\phong.vert", GL_VERTEX_SHADER);
	eff.add_shader("..\\resources\\shaders\\phong.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();



	mat4 P = cam->get_projection();
	mat4 V = cam->get_view();

	vec3 eyeP = cam->get_position();


	//root = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(10.0f, 10.0f, 10.0f), &meshes["plane"], &materials["plane"], &tex, &eff, P, V, eyeP, &light);

	ball = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(10.0f, 10.0f, 10.0f), &meshes["plane"], &materials["plane"], &tex, &eff, &light);

	Obj *box = new Obj(vec3(-10.0f, 2.5f, -30.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(0.5f, 0.5f, 0.5f), &meshes["box"], &materials["box"], &tex, &eff, &light);

	Obj *pyra = new Obj(vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &meshes["pyramid"], &materials["pyramid"], &tex, &eff, &light);

	ball->addChild(box, "box");
	box->addChild(pyra, "pyramid");

	list.push_back(ball);
	list.push_back(box);
	list.push_back(pyra);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Create a sphere
 //   sphere = mesh(geometry_builder::create_sphere(25, 25));


	//// add extra geom
	//mesh meshPlane;
	////meshPlane  = mesh(geometry_builder::create_plane());
	//meshPlane = mesh(geometry_builder::create_box());
	////meshes["box"] = mesh(geometry_builder::create_box());

	//material planeMat;
	//planeMat.set_diffuse(vec4(1.0f, 0.0f, 0.0f, 0.0f));
 //   planeMat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//planeMat.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//planeMat.set_shininess(2.0f);


	//materials["skybox"].set_diffuse(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//materials["ball"].set_diffuse(vec4(1.0f, 0.0f, 0.0f, 0.0f));


	////meshPlane.set_material(planeMat);

	//for (auto &e : materials)
	//{
	//	e.second.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//	e.second.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//	e.second.set_shininess(2.0f);
	//}


	//directional_light light;
	//// ambient intensity (0.3, 0.3, 0.3)
	//light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));

	//// Light colour white
	//light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//// Light direction (1.0, 1.0, -1.0)
	//light.set_direction(vec3(1.0f, 1.0f, -1.0f));

	//tex = texture("..\\resources\\textures\\checked.gif");


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

    // Load in shaders
   /* eff.add_shader("shader.vert", GL_VERTEX_SHADER);
    eff.add_shader("shader.frag", GL_FRAGMENT_SHADER);*/
    // Build effect

	/*eff.add_shader("..\\resources\\shaders\\phong.vert", GL_VERTEX_SHADER);
	eff.add_shader("..\\resources\\shaders\\phong.frag", GL_FRAGMENT_SHADER);
    eff.build();
*/

	
    // *********************
    // Load in skybox effect
    // *********************
	sky_eff.add_shader("..\\resources\\shaders\\skybox.vert", GL_VERTEX_SHADER);
	sky_eff.add_shader("..\\resources\\shaders\\skybox.frag", GL_FRAGMENT_SHADER);

    // Build effect
    sky_eff.build();

	root = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(100.0f, 100.0f, 100.0f), &skybox, &materials["skybox"], &tex, &sky_eff, &light);
	list.push_back(root);

	//ball = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(0.1f, 0.1f, 0.1f), &sphere, &materials["ball"], &tex, &eff, &light);
	////Obj *plane = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(0.0f, 0.0f, 0.0f), &meshPlane, &planeMat, &tex, &eff, &light);

	//
	////root->addChild(ball, "ball");
	//list.push_back(ball);


	// plane geometry not working
	//root->addChild(plane, "plane");
	//list.push_back(plane);

    return true;
}

bool update(float delta_time)
{
	
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_T))
		cam = cameraList[0];
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F))
		cam = cameraList[1];

	free_camera* freeCam = NULL;
	freeCam = dynamic_cast<free_camera*>(cam);

	if (freeCam)
	{
		GLFWwindow* window = renderer::get_window();

		// The ratio of pixels to rotation - remember the fov
		static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
		static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());

		glfwGetCursorPos(window, &new_x, &new_y);	// Get the current cursor position

		if (firstMouse)							 // if first mouse take cursor positons from initalised vars
		{
			current_x = xpos;
			current_y = ypos;
			firstMouse = false;
		}

		double delta_x = new_x - current_x;		 // Calculate delta of cursor positions from last frame
		double delta_y = new_y - current_y;

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


		glfwGetCursorPos(window, &current_x, &current_y);  // update cursor pos
	}


	cam->update(delta_time);  // update the camera
	
	bool sky = true;					// Set skybox flag so root position to camera position(camera in centre of skybox)
	root->update(root, mat4(1), sky);

    return true;
}

bool render()
{

	root->render(root, true);  // is sky true (enable/disable depth)

	ball->render(ball, false); // check -p;lane
 
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);

 //   // Bind effect
 //   renderer::bind(eff);
 //   // Create MVP matrix
 //   auto M = sphere.get_transform().get_transform_matrix();
	//auto V = cam->get_view();
	//auto P = cam->get_projection();
 //   auto MVP = P * V * M;
 //   // Set MVP matrix uniform
 //   glUniformMatrix4fv(
 //       eff.get_uniform_location("MVP"),
 //       1,
 //       GL_FALSE,
 //       value_ptr(MVP));
 //   // Render mesh
 //   renderer::render(sphere);

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

	for (int i = 0; i < list.size(); ++i)
	{
		delete list[i];
	}
	list.clear();


	for (int i = 0; i < cameraList.size(); ++i)
	{
		delete cameraList[i];
	}
	cameraList.clear();
}