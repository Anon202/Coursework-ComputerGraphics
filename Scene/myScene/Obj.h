#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;


class Obj
{
	mat4				mlocal;
	mat4				mworld;
	mesh*				m;
	material*			mat;
	texture*			tex;
	effect*				eff;
	directional_light*	light;

	Obj*				parent;

	vec3				cent;    // to create bounding spheres
	float r;

	map<string, Obj*>   children;  // map of children object pointers

	bool visible;

public:
	Obj	();
	Obj(vec3 pos,
		vec3 rot, float theta,
		vec3 scale,
		mesh* me,
		material* mate,
		texture* texture,
		effect* eff,
		directional_light* light);

	void update(Obj* root, mat4 m, bool sky);

	void render(Obj* root);

	void addChild(Obj* child, string name);

};

