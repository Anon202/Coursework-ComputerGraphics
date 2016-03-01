#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>
#include "GenerateBack.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;

class GenerateBack;
class Obj;				// forward declaration of object


enum objType { sky, terrn, waterObj, object, pointLightObj, forShade, spotty }; //  enum
class SceneManager
{
	
public:
	
	Obj* skybx;
	Obj* root;

	GenerateBack* terr;

	
	directional_light* light;
	point_light* pointLight;
	spot_light* spot;

	camera* cam;

	vector<camera*> cameraList;
	vector<vector<texture*>> texList;
	
	vector<effect*> effectList;

	vector<Obj*> list;

	effect* shadow_eff;

	map<string, mesh> meshes;
	map<string, material> materials;
	map<string, cubemap> cubemaps;

	shadow_map shadow;

	vec3 planeNormals[6]; // var for storing view frustrum plane normals.
	
	double initialX;				// vars for mouse positions initial
	double initialY;	
	double current_x;			    // var for current mouse pos
	double current_y;

	bool firstMouse;

	SceneManager(double initialMouseX, double initialMouseY);
	~SceneManager();
	void Create();
	void calculateFrustrum();
//	void intersection();
};

