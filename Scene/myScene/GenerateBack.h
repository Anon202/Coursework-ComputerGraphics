/*
 *	GenerateBack.h - header file for generation of background, and other objects
 *	Terrain, skybox, own geometry prism
 *
 */

#pragma once

#include <graphics_framework.h>
#include <glm\glm.hpp>
#include "SceneManager.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

class GenerateBack
{
public:
	
	void generate_terrain(
		vector<geometry> &geom,
		const texture &height_map,
		unsigned int width,
		unsigned int depth,
		float height_scale,
		vector<vec3> &centre);

	void generate_skybox(
		mesh &skybox,
		cubemap &cube_map,
		bool skyOuter);

	void generate_bar(geometry &geom);
};



