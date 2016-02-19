#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>
#include "SceneManager.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

class Obj
{
	mat4				mlocal;
	mat4				mworld;
	mesh*				m;
	material*			mat;
	effect*				eff;
	directional_light*	light;

	Obj*				parent;

	vec3				cent;    // to create bounding spheres
	float r;
	//texture*			tex;

	vector<texture*> tex;
	map<string, Obj*>   children;  // map of children object pointers

	bool visible;

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

	void update(Obj* root, mat4 m);

	void render(Obj* root);

	void addChild(Obj* child, string name);

};

