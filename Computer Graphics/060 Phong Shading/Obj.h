#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

class Obj
{
	Obj *				parent;

	mat4				mlocal;
	mat4				mworld;
	mesh*				m;
	material*			mat;
	texture*			tex;

	//////
	effect*				eff;
	//mat4				P;
	//mat4				V;
	//vec3				eyeP;

	camera*				myCam;
	directional_light*	light;

	vec3				cent;
	float r;

	map<string, Obj*> children;

	bool visible;

public:
	Obj				();
	Obj(vec3 pos, 
		vec3 rot, float theta, 
		vec3 scale, 
		mesh* me, 
		material* mate, 
		texture* texture, 
		effect* eff, 
		//mat4 P,
		//mat4 V, 
		//vec3 eyeP,
		camera* c,
		directional_light* light);

	static
	void update		(Obj* root,		mat4 m);

	static
	void render		(Obj* root);
	//void render(effect& eff, mat4& PV, vec3& eyeP, directional_light& light);
	
	void addChild(Obj* child, string name);

};

