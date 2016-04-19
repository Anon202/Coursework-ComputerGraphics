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


enum objType { sky, terrn, waterObj, object, pointLightObj, forShade, spotty, glassOb, movingObject }; //  enum
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

	double initialX;				// vars for mouse positions initial
	double initialY;
	double current_x;			    // var for current mouse pos
	double current_y;

	float myTime; // float for passing in uniform for water and point light's vertex displacement + movement

	geometry screen_quad;
	frame_buffer frame;
	frame_buffer vigFrame;
	frame_buffer ssaoframe;

	frame_buffer blurTargetA;
	frame_buffer blurTargetB;

	effect greyEff;
	effect simpleTex;
	effect ssao_Position;
	effect blurEff;
	effect vignetteEff;

	geometry frustrumGeom;

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

	// getters/setters for values	
	bool getDebugBool(){ return debug; }
	void setDebugBool(const bool &value){ debug = value; }

	bool getFixCullBool(){ return fixCull; }
	void setFixCullBool(const bool &value){ fixCull = value; }

	bool getfirstMouse(){ return firstMouse; }
	void setfirstMouse(const bool &value){ firstMouse = value; }

	double getInitialX(){ return initialX; }
	double getInitialY(){ return initialY; }

	void setCurrX(const double &value) { current_x = value; }
	double getCurrX() { return current_x; }

	void setCurrY(const double &value) { current_y = value; }
	double getCurrY() { return current_y; }

	void incrementMyTime(const float &value){ myTime += value;	}
	float getMyTime(){ return myTime; }

	bool getGreyBool() { return grey; }
	void setGreyBool(const bool &value) { grey = value; }

	bool getBlurBool() { return blur; }
	void setBlurBool(const bool &value) { blur = value; }

	bool getVigBool() { return vig; }
	void setVigBool(const bool &value) { vig = value; }

	bool getGUIBool() { return gui; }
	void setGUIBool(const bool &value) { gui = value; }

	bool getSSAO() { return ssao; }
	void setSSAO(const bool &value) { ssao = value; }

	geometry getRadiGeom() { return radiusGeom; }
	effect* getRadEff() { return rad_eff; }

	geometry getFrustrumGeom() { return frustrumGeom; }
	
	// post- processing getters

	geometry getScreenQuad() { return screen_quad; }

	frame_buffer* getFrame(){ return &frame; }
	effect* getGreyEffect() { return &greyEff; }

	frame_buffer* getBlurA() { return &blurTargetA; }
	frame_buffer* getBlurB() { return &blurTargetB; }
	effect* getBlurEffect() { return &blurEff; }
	
	effect* getSimpleTexEffect() { return &simpleTex; }

	frame_buffer* getSSAOFrame() { return &ssaoframe; }
	effect* getSSAOPosEffect() { return &ssao_Position; }

	frame_buffer* getVigFrame() { return &vigFrame; }
	effect* getVignetteEffect() { return &vignetteEff; }
};

