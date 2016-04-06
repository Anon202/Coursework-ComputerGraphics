/*
*	Obj.h - header file for main object class
*	All items within the scene are instances of an object, including the skybox and terrain blocks
*	Used for spacial partioning and creating a hierarchy of meshes.
*/

#pragma once
#include "SceneManager.h"

class Obj
{
	// vars for objects matrices
	mat3				normalMatrix;
	mat4				mlocal;

	mat4				translateMatrix;
	mat4				scaleMatrix;
	mat4				rotationMatrix;

	material*			mat;	// material of object
	effect*				eff;	// which effect it uses for lighting

	Obj*				parent;

	float				radius;
	vec3				furthestPoint;

	float		     	theta;		     // angle to update by every frame
	float				angleIncrement;  // used to store angle increment over time

	cubemap				myCubemap;
	vector<texture*>	tex;
	map<string, Obj*>   children;  // map of children object pointers

	string				myName;
	vec3				rotV;
	
	vec3				centreT;  // vector to hold centre point of terrain object


	// variables for storing translation and scale information for updating over time/hierarchy
	vec3				translationAdjustment;
	vec3				initialTranslation;
	vec3				totalTranslation;
	vec3				maxTranslation;
	float				scaleFactor;

	// bools to determine whether to update the trans/scale matrices
	bool				transUpdate = false;
	bool				scaleUpdate = false;

public:
	vec3				cent;    // to create bounding spheres
	mat4				mworld;
	bool				visible;
	mesh*				m;
	float			    myType;

	Obj	();

	// normal object constructor 
	Obj(vec3 pos,						// position (translation coordinates from parent (origin if not parented)
		vec3 rot, float theta,			// rotation axis and angle
		vec3 scale,						// initial scale transformation 
		mesh* mesh,						// mesh
		material* material,				// material 
		vector<texture*> texture,		// vector list of textures (many objects are associated with more than one texture for blend/norm map)
		effect* eff,					// shader program
		float myType);					// enum for setting flags 

	Obj(vec3 rot, float theta, // skybox object constructor
		vec3 scale,
		mesh* me,
		cubemap skybox,
		effect* eff);

	void calculateSphere();  // calculates bounding sphere for object
	void intersection();	 // intersection test for view frustrum

	void update(Obj* parent, float time);  // update function, if no parent NULL is entered

	void render();			// render function

	void renderGlass();		// separate render function for glass object as needs to be rendered last after the opaque objects

	void addChild(Obj* child, string name);

	vec4  getWorldPos();			// calculates the centre postion of object in worldspace for bounding sphere calculation + radii drawing
	float getRadius();				// calculatees object's radius (longest length)
	void  setName(string name);

	void setCenterTerr(vec3 cent);  // method to set terrain center as calculated differently from the sphere method

	void setScaleFactor(float scaleFactor);  // used to scale the object over time from sin wave

	void setTranslationParams(vec3 move, vec3 max);  // used to set moveby value and max distance moved for translating objects over time
	string getName() { return myName; };
};

