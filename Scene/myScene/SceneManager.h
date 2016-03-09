#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>
#include "GenerateBack.h"
#include "Light.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

class GenerateBack;
class Obj;				// forward declaration of object


enum objType { sky, terrn, waterObj, object, pointLightObj, forShade, spotty, glassOb }; //  enum
enum planeNum { farN, topN, leftN, nearN, bottN, rightN };
enum planePoints {ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr };
class SceneManager
{
	
public:
	
	Obj* skybx;

	GenerateBack* generator;

	//directional_light* light;
	//point_light* pointLight;
	//spot_light* spot;

	camera* cam;

	vector<camera*> cameraList;
	vector<Light*> lightList;
	vector<vector<texture*>> texList;
	
	vector<effect*> effectList;

	vector<Obj*> list;

	vector<Obj*> lightObjects;

	effect* shadow_eff;
	effect* rad_eff;

	map<string, mesh> meshes;
	map<string, material> materials;
	map<string, cubemap> cubemaps;

	shadow_map shadow;

	vec3 planeNormals[6]; // var for storing view frustrum plane normals.
	vec3 planePoints[8];

	geometry frustrumGeom;


	bool debug;
	bool fixCull;

	double initialX;				// vars for mouse positions initial
	double initialY;	
	double current_x;			    // var for current mouse pos
	double current_y;

	bool firstMouse;

	Obj* mySpot;
	
	geometry radiusGeom;

	SceneManager(double initialMouseX, double initialMouseY);
	~SceneManager();
	void Create();
	void calculateFrustrum();
	effect* createEffect(char vertPath[], char fragPath[], char partPath1[], char partPath2[]);
};

