#include "SceneManager.h"


SceneManager::SceneManager(double initialMouseX, double initialMouseY)
{
	cam = NULL;
	skybx = NULL;
	root = NULL;
	terr = NULL;

	
	// copy vars
	initialX = initialMouseX;
	initialY = initialMouseY;

	current_x = 0;   /// initialise current position of mouse
	current_y = 0;

	firstMouse = false;

	// initialise shadow map with screen size
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());

}

void SceneManager::Create()
{
	
	light = new directional_light; 
	light->set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));

	// Light colour white
	light->set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Light direction (1.0, 1.0, -1.0)
	light->set_direction(vec3(1.0f, 1.0f, -1.0f));

	
	pointLight = new point_light;
	pointLight->set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	pointLight->set_range(20);
	pointLight->set_position(vec3(31.5, 35.75, 63.0));
	pointLight->set_constant_attenuation(0.5f);
	pointLight->set_linear_attenuation(0.2f);
	pointLight->set_quadratic_attenuation(0.01f);

	spot = new spot_light;
	spot->set_position(vec3(-30.5, 200.0, 150.0));
	spot->set_direction(normalize(vec3(-1, -1, 0)));
	spot->set_light_colour(vec4(1.0, 1.0, 1.0, 1.0));
	spot->set_range(200);
	spot->set_power(50);
}

SceneManager::~SceneManager()
{

	//for (uint i = 0; i < list.size(); ++i)
	//	delete list[i];

	//list.clear();

	delete light;
	light = NULL;

	delete pointLight;
	pointLight = NULL;
	
	delete spot;
	spot = NULL;

	for (int i = 0; i < effectList.size(); ++i)
		delete effectList[i];

	effectList.clear();

	for (int i = 0; i < cameraList.size(); ++i)
		delete cameraList[i];

	cameraList.clear();

	for (int i = 0; i < texList.size(); ++i)			// texlist is a vector list of a vector list of texture pointers
	{
		for (int j = 0; j < texList[i].size(); ++j)     // for each element in texlist delete each element of list inside
			delete texList[i][j]; 
	}
	texList.clear();
}