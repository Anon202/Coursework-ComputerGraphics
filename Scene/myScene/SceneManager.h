/*
 * Scene Manager header file for class, to store scene-wide lists of objects/light pointers, etc.
 * Also calculates view frustrum.
 */


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


enum objType { sky, terrn, waterObj, object, pointLightObj, spotty, glassOb, movingObject }; //  enum
enum planeNum { farN, topN, leftN, nearN, bottN, rightN };
enum planePointNames {ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr };
enum lightTypes { direction, yellP, blueP, pinkP, spot };

class SceneManager
{
private:

	void createLights();

	bool debug;			 // flag for turning on polygon mode to render outlines
	bool fixCull;		 // flag for fixing view culling
	bool firstMouse;	 // flag for first mouse (for free camera)
	bool grey;
	bool blur;
	bool ssao;
	bool gui;
	bool vig;
	bool bloom;

	double initialX;				// vars for mouse positions initial
	double initialY;
	double current_x;			    // var for current mouse pos
	double current_y;

	float myTime; // float for passing in uniform for water and point light's vertex displacement + movement

	geometry screen_quad;
	frame_buffer screenFrame;
	frame_buffer vigFrame;
	frame_buffer ssaoframe;

	frame_buffer blurTargetA;
	frame_buffer blurTargetB;
	frame_buffer finalBlur;

	effect greyEff;
	effect simpleTex;
	effect ssao_Position;
	effect blurEff;
	effect vignetteEff;
	effect bloomEff;
	effect bloomFinalEff;

	geometry frustrumGeom;

	// post processing render functions
	void renderMyBloom();
	void renderVignette();
	void renderBlur(const bool &bloom);
	void renderRadii();
	void renderGreyScale();

public:
	void initQuad();
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

	effect* shad_eff;
	effect* rad_eff;

	map<string, mesh> meshes;
	map<string, material> materials;
	map<string, cubemap> cubemaps;

	shadow_map shadow;

	geometry radiusGeom;


	effect	 frustrumEffect;

	vec3 planeNormals[6];		// var for storing view frustrum plane normals.
	vec3 planePoints[8];


	effect* createEffect(const char vertPath[], const char fragPath[], const char partPath1[], const char partPath2[]);
	SceneManager(double initialMouseX, double initialMouseY);  // constructor takes in initial mouse positions for free camera setup
	~SceneManager();// deconstructor frees lists

	// recalculates view frustrum from current camera positions
	void calculateFrustrum();
	void generateFrustrumPlanes();



	void incrementMyTime(const float &value){ myTime += value;	}
	float getMyTime(){ return myTime; }


	geometry getRadiGeom() { return radiusGeom; }
	effect* getRadEff() { return rad_eff; }

	geometry getFrustrumGeom() { return frustrumGeom; }
	

	void renderScene();

	void updateLightPositions();
	void updateScene(float delta_time);

	void toggleDebug();
	void toggleBloom();
	void toggleBlur();
	void toggleVignette();
	void toggleGrey();
	void toggleCull();
};

