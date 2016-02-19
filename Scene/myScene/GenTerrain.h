#pragma once
class GenTerrain
{
public:
	GenTerrain();

};



void generate_terrain(geometry &geom, const texture &height_map, unsigned int width, unsigned int depth, float height_scale);