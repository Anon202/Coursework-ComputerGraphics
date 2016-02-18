#include "SceneManager.h"


SceneManager::SceneManager()
{
	cam = NULL;
	root = NULL;
	plane = NULL;
	
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

	for (int i = 0; i < texList.size(); ++i)
		delete texList[i];

	texList.clear();
}