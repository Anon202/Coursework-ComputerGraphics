#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>
#include "GenerateBack.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;

class GenerateBack;
class Obj;				// forward declaration of object


enum objType { sky, terrn, waterObj, object }; //  enum
class SceneManager
{
	
public:
	
	Obj* skybx;
	Obj* root;

	GenerateBack* terr;

	directional_light* light;

	camera* cam;

	vector<camera*> cameraList;
	vector<vector<texture*>> texList;
	
	vector<effect*> effectList;

	vector<Obj*> list;


	map<string, mesh> meshes;
	map<string, material> materials;
	map<string, cubemap> cubemaps;
	
	double initialX;	// vars for mouse positions initial
	double initialY;
	double current_x;   // var for current mouse pos
	double current_y;

	bool firstMouse;

	SceneManager(double initialMouseX, double initialMouseY);
	~SceneManager();
	void Create();
};

