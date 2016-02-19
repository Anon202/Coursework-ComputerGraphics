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
	GenerateBack();
	void generate_terrain(
		geometry &geom,
		const texture &height_map,
		unsigned int width,
		unsigned int depth,
		float height_scale);

	void generate_skybox(
		mesh &skybox,
		cubemap &cube_map,
		float skyNum);

};



