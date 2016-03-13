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


enum objType { sky, terrn, waterObj, object, pointLightObj, forShade, spotty, glassOb, movingObject }; //  enum
enum planeNum { farN, topN, leftN, nearN, bottN, rightN };
enum planePoints {ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr };
class SceneManager
{
	
public:
	
	Obj* skybx;

	GenerateBack* generator;

	camera* cam;

	vector<camera*> cameraList;
	vector<Light*> lightList;
	vector<vector<texture*>> texList;
	
	vector<effect*> effectList;  // list of all effect pointers for freeing

	vector<Obj*> list;
	vector<Obj*> transparentObjects;

	vector<Obj*> lightObjects;

	effect* shadow_eff;
	effect* rad_eff;

	map<string, mesh> meshes;
	map<string, material> materials;
	map<string, cubemap> cubemaps;

	shadow_map shadow;

	vec3 planeNormals[6]; // var for storing view frustrum plane normals.
	vec3 planePoints[8];

	geometry radiusGeom;		// geometry for scene debugging 
	geometry frustrumGeom;
	effect	 frustrumEffect;

	bool debug;
	bool fixCull;

	double initialX;				// vars for mouse positions initial
	double initialY;	
	double current_x;			    // var for current mouse pos
	double current_y;

	bool firstMouse;	

	SceneManager(double initialMouseX, double initialMouseY);  // constructor takes in initial mouse positions for free camera setup
	~SceneManager();										   // deconstructor frees lists
	void createLights();
	void calculateFrustrum();
	effect* createEffect(char vertPath[], char fragPath[], char partPath1[], char partPath2[]);

	float myTime; // float for passing in uniform for water and point light's vertex displacement + movement
};

