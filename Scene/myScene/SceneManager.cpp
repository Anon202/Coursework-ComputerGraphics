#include "SceneManager.h"


SceneManager::SceneManager(double initialMouseX, double initialMouseY)
{
	cam = NULL;
	root = NULL;
	plane = NULL;
	

	// copy vars
	initialX = initialMouseX;
	initialY = initialMouseY;

	current_x = 0;   /// initialise current position of mouse
	current_y = 0;

	firstMouse = false;
}

void SceneManager::Create()
{

	light = new directional_light; 
	light->set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));

	// Light colour white
	light->set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Light direction (1.0, 1.0, -1.0)
	light->set_direction(vec3(1.0f, 1.0f, -1.0f));

}

void SceneManager::Release()
{
	delete light;
	light = NULL;

	for (int i = 0; i < list.size(); ++i)
		delete list[i];

	list.clear();


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