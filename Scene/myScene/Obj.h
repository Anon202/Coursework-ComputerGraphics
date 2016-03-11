#pragma once


#include "SceneManager.h"



class Obj
{
	mat3				normalMatrix;
	mat4				mlocal;

	mat4				translateMatrix;
	mat4				scaleMatrix;
	mat4 rotationMatrix;

	material*			mat;
	effect*				eff;

	Obj*				parent;

	float				radius;
	vec3				furthestPoint;

	float theta;		   // angle to update by every frame
	float angleIncrement;  // used to store angle increment over time

	cubemap				myCubemap;
	vector<texture*>	tex;
	map<string, Obj*>   children;  // map of children object pointers

	string				myName;
	vec3 rotV;
	
	vec3 centreT;
public:
	vec3				cent;    // to create bounding spheres
	mat4				mworld;
	bool				visible;
	mesh*				m;
	float			    myType;

	Obj	();
	Obj(vec3 pos,
		vec3 rot, float theta,
		vec3 scale,
		mesh* me,
		material* mate,
		vector<texture*> texture,
		effect* eff,
		float myType);

	Obj(vec3 rot, float theta, // skybox object constructor
		vec3 scale,
		mesh* me,
		cubemap skybox,
		effect* eff);

	void calculateSphere();
	void intersection();

	void update(Obj* parent, float time);

	void render();
	void renderGlass();

	void addChild(Obj* child, string name);

	vec4  getWorldPos();
	float getRadius();
	void  setName(string name);

	void setCenterTerr(vec3 cent);

	~Obj() { }

};

