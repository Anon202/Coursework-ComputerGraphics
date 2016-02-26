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

	vec3				cent;    // to create bounding spheres
	float r;
	//texture*			tex;

	float theta;

	vector<texture*>	tex;
	map<string, Obj*>   children;  // map of children object pointers

	bool visible;
	
	vec3 rotV;
	float myType;

public:
	Obj	();
	Obj(vec3 pos,
		vec3 rot, float theta,
		vec3 scale,
		mesh* me,
		material* mate,
		vector<texture*> texture,
		effect* eff,
		float myType);



	void update(Obj* parent, float time);

	void render();

	void addChild(Obj* child, string name);

	~Obj() { }

};

