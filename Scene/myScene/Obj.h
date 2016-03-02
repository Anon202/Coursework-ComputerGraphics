#pragma once


#include "SceneManager.h"



class Obj
{
	mat4				mlocal;
	mat4				mworld;
	mesh*				m;
	material*			mat;
	effect*				eff;

	Obj*				parent;

	float				radius;
	vec3				furthestPoint;
	//texture*			tex;

	float theta;

	vector<texture*>	tex;
	map<string, Obj*>   children;  // map of children object pointers

	vector<vec3>		vertPos;
	
	vec3 rotV;
	float myType;

public:
	vec3				cent;    // to create bounding spheres

	bool				visible;
	

	Obj	();
	Obj(vec3 pos,
		vec3 rot, float theta,
		vec3 scale,
		mesh* me,
		material* mate,
		vector<texture*> texture,
		effect* eff,
		float myType,
		vector<vec3> vertPos);

	void calculateSphere();
	void intersection();

	void update(Obj* parent, float time);

	void render();

	void addChild(Obj* child, string name);

	vec3  getWorldPos();
	float getRadius();

	~Obj() { }

};

