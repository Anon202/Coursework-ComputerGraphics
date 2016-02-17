#include <graphics_framework.h>
#include <glm\glm.hpp>

#include "Obj.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;

// materials

map<string, material> materials;

effect eff;
texture tex;
//free_camera cam;

camera* cam = NULL;

vector<camera*> cameraList;

directional_light light;

Obj* root = NULL;
vector<Obj*> list;

//map<string, Obj> tree;

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


	//free_camera cam;

	cam = new target_camera();
	cameraList.push_back(cam);
	// create target camera
	cam->set_position(vec3(50.0f, 10.0f, 50.0f));
	cam->set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	

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
	
	
	meshes["tetra"].get_transform().scale = vec3(4.0f, 4.0f, 4.0f);
	meshes["tetra"].get_transform().translate(vec3(-30.0f, 10.0f, -10.0f));
	
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
	materials["box"].set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	
	// Green tetra
	materials["tetra"].set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	
	// Blue pyramid
	materials["pyramid"].set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// Yellow disk
	materials["disk"].set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	
	// Magenta cylinder
	materials["cylinder"].set_diffuse(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	
	// Cyan sphere
	materials["sphere"].set_diffuse(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	
	// White torus
	materials["torus"].set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	for (auto &e : materials)
	{
		e.second.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
		e.second.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		e.second.set_shininess(2.0f);
	}

	meshes["box"].set_material(materials["box"]);
	meshes["tetra"].set_material(materials["tetra"]);
	meshes["pyramid"].set_material(materials["pyramid"]);
	meshes["disk"].set_material(materials["disk"]);
	meshes["cylinder"].set_material(materials["cylinder"]);
	meshes["sphere"].set_material(materials["sphere"]);
	meshes["torus"].set_material(materials["torus"]);

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

	root = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(10.0f, 10.0f, 10.0f), &meshes["plane"], &materials["plane"], &tex, &eff, &light);

	Obj *box = new Obj(vec3(-10.0f, 2.5f, -30.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(0.5f, 0.5f, 0.5f), &meshes["box"], &materials["box"], &tex, &eff, &light);
	
	Obj *pyra = new Obj(vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &meshes["pyramid"], &materials["pyramid"], &tex, &eff, &light);
	
	root->addChild(box, "box");
	box->addChild(pyra, "pyramid");

	list.push_back(root);
	list.push_back(box);
	list.push_back(pyra);

	return true;
}

bool update(float delta_time)
{// The ratio of pixels to rotation - remember the fov
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


	if (glfwGetKey(renderer::get_window(), GLFW_KEY_U))
		cam = cameraList[0];
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_J))
		cam = cameraList[1];

	// *************************
	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	// *************************
	free_camera* fcam = NULL;
	fcam = dynamic_cast<free_camera*>(cam);



	if (fcam)
	{
		fcam->rotate((float)delta_x, (float)delta_y);


		// *******************************
		// Use keyboard to move the camera
		// - WSAD
		// *******************************
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
			fcam->move(vec3(0.0f, 0.0f, 1.0f));
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
			fcam->move(vec3(-1.0f, 0.0f, 0.0f));
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
			fcam->move(vec3(1.0f, 0.0f, 0.0f));
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
			fcam->move(vec3(0.0f, 0.0f, -1.0f));
	}
	else
	{
		
	}



	// ***********
	// Move camera
	// ***********

	// *****************
	// Update the camera
	// *****************
	cam->update(delta_time);


	// *****************
	// Update cursor pos
	// *****************
	glfwGetCursorPos(window, &current_x, &current_y);

	//tree["root"].update(&tree["root"], mat4(1.0f));

	root->update(root, mat4(1) );

	return true;
}

bool render()
{
	root->render(root);

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