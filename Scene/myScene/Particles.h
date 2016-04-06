#pragma once
#include "SceneManager.h"

using namespace std::chrono;



struct particle
{
	vec3 position;
	vec3 velocity;
	vec2 lifetime;
};

void initialiseParticles(int numofParticles);
void updateParticles(float delta_time);
void renderParticles();
