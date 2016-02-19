#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>
#include "GenTerrain.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;

class GenTerrain;
class Obj;				// forward declaration of object


enum objType { sky, terrn, waterObj, object }; //  enum
class SceneManager
{
	
public:
	
	Obj* root;
	Obj* plane;

	GenTerrain* terr;

	directional_light* light;

	camera* cam;

	vector<camera*> cameraList;
	vector<vector<texture*>> texList;
	
	

	vector<Obj*> list;


	map<string, mesh> meshes;
	map<string, material> materials;

	
	double initialX;	// vars for mouse positions initial
	double initialY;
	double current_x;   // var for current mouse pos
	double current_y;

	bool firstMouse;


	//	static void Create();
	//public:
	//	static void Release();
	//
	//	static void Update();
	//
	//	static void Draw();
	//


	SceneManager(double initialMouseX, double initialMouseY);
	~SceneManager();
	void Create();
	//void Release();

private:
	//static Obj* root;
	//static effect* basic;
	
};

