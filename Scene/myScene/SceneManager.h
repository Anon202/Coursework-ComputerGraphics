#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>



using namespace std;
using namespace graphics_framework;
using namespace glm;

class Obj;				// forward declaration of object
class SceneManager
{
	
public:
	
	Obj* root;
	Obj* plane;

	directional_light* light;

	camera* cam;

	vector<camera*> cameraList;
	vector<vector<texture*>> texList;
	enum objType { sky, terrain, water, object }; //  enum
	

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

	void Create();
	void Release();

private:
	//static Obj* root;
	//static effect* basic;
	
};

