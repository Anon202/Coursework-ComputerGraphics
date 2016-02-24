#pragma once


#include "SceneManager.h"



class Obj
{
	mat4				mlocal;
	mat4				mworld;
	mesh*				m;
	material*			mat;
	effect*				eff;
	directional_light*	light;
	point_light*		pointLight;

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
		directional_light* light,
		float myType);

	Obj(vec3 pos,
		vec3 rot, float theta,
		vec3 scale,
		mesh* me,
		material* mate,
		vector<texture*> texture,
		effect* eff,
		point_light* light,
		float myType);

	void update(Obj* parent, float time);

	void render();

	void addChild(Obj* child, string name);

};

