#include "GenerateBack.h"


GenerateBack::GenerateBack()
{
	
}

void GenerateBack::generate_skybox(mesh &skybox, cubemap &cube_map, bool skyOuter)
{
	geometry geom;
	geom.set_type(GL_QUADS);
	vector<vec3> positions
	{
		// Face 4
		vec3(-1.0, 1.0, -1.0), // 5
		vec3(-1.0, 1.0, 1.0), //1
		vec3(-1.0, -1.0, 1.0), // 2
		vec3(-1.0, -1.0, -1.0), //7

		// Face 3
		vec3(1.0, 1.0, 1.0), // 4
		vec3(1.0, 1.0, -1.0), //6
		vec3(1.0, -1.0, -1.0), // 8
		vec3(1.0, -1.0, 1.0), // 3

		// Face 5
		vec3(-1.0, 1.0, -1.0), // 5
		vec3(1.0, 1.0, -1.0), //6
		vec3(1.0, 1.0, 1.0), // 4
		vec3(-1.0, 1.0, 1.0), //1

		// Face 6
		vec3(-1.0, -1.0, 1.0), // 2
		vec3(1.0, -1.0, 1.0), // 3
		vec3(1.0, -1.0, -1.0), // 8
		vec3(-1.0, -1.0, -1.0), //7

		// Face 2
		vec3(-1.0, -1.0, -1.0), //7
		vec3(1.0, -1.0, -1.0),  // 8
		vec3(1.0, 1.0, -1.0),   //6
		vec3(-1.0, 1.0, -1.0),  // 5

		// Face 1
		vec3(1.0, -1.0, 1.0),	// 3
		vec3(-1.0, -1.0, 1.0),	// 2
		vec3(-1.0, 1.0, 1.0),	//1
		vec3(1.0, 1.0, 1.0),    // 4
	};

	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);  // add position to buffer
	skybox = mesh(geom);


	// Load the cubemap
	// - create array of six filenames +x, -x, +y, -y, +z, -z


	// two cubemaps sky inner and outer stored in different folders with different file extensions
	string folderName, ext;

	if (skyOuter)					// use valley cubemap
	{
		folderName = "valley";
		ext = "tga";
	}
	else							// use cloud cubemap
	{
		folderName = "myClouds";
		ext = "png";
	}

	// add textures to array in correct order
	array<string, 6> filenames =
	{
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\posx." + ext,
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\negx." + ext,
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\posy." + ext,
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\negy." + ext,
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\posz." + ext,
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\negz." + ext
	};


	// Create cube_map from array
	cube_map = cubemap(filenames);

}

void addToBuffers(geometry &currentGeom, vector<vec3> &positions, vector<unsigned int> &indices, vector<vec3> &normals, vector<vec2> &tex_coords, vector<vec4> &tex_weights)
{
	// method for adding to buffers, for the terrain block data

	currentGeom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	currentGeom.add_index_buffer(indices);
	currentGeom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
	currentGeom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	currentGeom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);
}

void GenerateBack::generate_terrain(vector<geometry> &geom, const texture &height_map, unsigned int width, unsigned int depth, float height_scale, vector<vec3> &centre)
{
	// Contains our position data
	vector<vec3> positions;
	// Contains our normal data
	vector<vec3> normals;
	// Contains our texture coordinate data
	vector<vec2> tex_coords;
	// Contains our texture weights
	vector<vec4> tex_weights;
	// Contains our index data
	vector<unsigned int> indices;

	//vector<vec3> centre;

	geometry geomTemp[4];  // temporary geometry values

	// ***************************************
	// Extract the texture data from the image
	// ***************************************
	glBindTexture(GL_TEXTURE_2D, height_map.get_id());
	auto data = new vec4[height_map.get_width() * height_map.get_height()];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (void*)data);


	// Determine ratio of height map to geometry
	float width_point = static_cast<float>(width) / static_cast<float>(height_map.get_width());
	float depth_point = static_cast<float>(depth) / static_cast<float>(height_map.get_height());

	// Point to work on
	vec3 point;

	GLint halfWidth = height_map.get_width() / 2;
	GLint halfHeight = height_map.get_height() / 2;
	 
	int start[4] =		 { 0,  halfWidth -1,     		  0, halfWidth -1 };
	int startHeight[4] = { 0,	halfHeight -1 , 	 halfHeight -1,		    0 };

	int end[4] =		{ halfWidth,  halfWidth * 2,	  halfWidth, halfWidth *2 };
	int endHeight[4] = { halfHeight, (halfHeight * 2) -1,  (halfHeight *2) -1,   halfHeight};

	for (unsigned int j = 0; j < 4; ++j)  // loop for four quadrants of terrain
	{

		// Part 1 - Iterate through each point, calculate vertex and add to vector
		// use starting and ending values of terrain (0 -> halfwidth, halfwidth -> 2 etc.)
		for (int x = start[j]; x < end[j]; ++x)
		{

			// Calculate x position of point
			float w2 = width / 2.0f;
			point.x = -w2 + (width_point * x);

			// loop through heights
			for (int z = startHeight[j]; z < endHeight[j]; ++z)
			{
				// Calculate z position of point
				float d2 = depth / 2.0f;
				point.z = -d2 + (depth_point*z);

				// Y position based on red component of height map data
				point.y = data[(z * height_map.get_height()) + x].y * height_scale;

				// Add point to position data
				positions.push_back(point);
			}
		}


		// Add index data
		for (unsigned int x = 0; x < halfWidth - 1; ++x)
		{
			for (unsigned int y = 0; y < halfHeight - 1; ++y)
			{

				// Get four corners of patch
				unsigned int top_left = (y * halfWidth) + x;
				unsigned int top_right = (y * halfWidth) + x + 1;
				unsigned int bottom_left = ((y + 1)*halfWidth) + x;
				unsigned int bottom_right = ((y + 1)*halfWidth) + x + 1;

				// Push back indices for triangle 1
				indices.push_back(top_left);
				indices.push_back(bottom_right);
				indices.push_back(bottom_left);

				// Push back indices for triangle 2
				indices.push_back(top_left);
				indices.push_back(top_right);
				indices.push_back(bottom_right);

			}
		}


		// Resize the normals buffer according to the size of the positions
		normals.resize(positions.size());
		
		// Calculate normals for the height map using the cross product of the corners 
		// loop for indicies/3 as three for each triangle
		for (unsigned int i = 0; i < indices.size() / 3; ++i)
		{
			// Get indices for the triangle
			int idx1 = indices[i * 3];
			int idx2 = indices[i * 3 + 1];
			int idx3 = indices[i * 3 + 2];

			// Calculate two sides of the triangle
			vec3 side1 = positions[idx1] - positions[idx3];
			vec3 side2 = positions[idx1] - positions[idx2];

			// Normal is cross product of these two sides
			vec3 n = cross(side2, side1);

			// Add to normals in the normal buffer using the indices for the triangle
			normals[idx1] += n;
			normals[idx2] += n;
			normals[idx3] += n;

		}

		// NORMALIZE THE NORMALS
		for (auto &n : normals)
		{
			n = normalize(n);
		}

		/* Add texture coordinates for geometry
		 *
		 * increments used for keeping track of each segment within the whole loop.
		 * if it's the first time around texture coordinates will be from zero to half the width of the map,
		 * second time (j=1) the width is increased by one as it is from the middle of map to the end of the map 
		 */
		int incrementX = 0;

		if (j == 1)
		{
			incrementX++;
		}

		else if (j == 3)
		{
			incrementX++;
		}
		
		for (unsigned int x = 0; x < halfWidth + incrementX; ++x)
		{
			for (unsigned int z = 0; z < halfHeight; ++z)
			{
				vec2 v;
				v.x = width_point * x;
				v.y = depth_point * z;
				tex_coords.push_back(v);
			}
		}


		// Calculate texture weights for each vertex
		for (unsigned int x = start[j]; x < end[j]; ++x)
		{
			for (unsigned int z = startHeight[j]; z < endHeight[j]; ++z)
			{

				// Calculate tex weight depending on percentages of the total height 
				vec4 w;
				w.x = clamp((1.0f - (abs(data[height_map.get_width() * z + x].y - 0.0f) / 0.0625f)), 0.0f, 1.0f);
				w.y = clamp((1.0f - (abs(data[height_map.get_width() * z + x].y - 0.05f) / 0.125f)), 0.0f, 1.0f);
				w.z = clamp((1.0f - (abs(data[height_map.get_width() * z + x].y - 0.08f) / 0.5625f)), 0.0f, 1.0f);
				w.w = clamp((1.0f - (abs(data[height_map.get_width() * z + x].y - 0.8f) / 0.25f)), 0.0f, 1.0f);


				// Sum the components of the vector
				float total = w.x + w.y + w.z + w.w;

				// Divide weight by sum
				w = w / total;

				// Add tex weight to weights

				tex_weights.push_back(w);
			}
		}
		
		// add buffers to geometry
		addToBuffers(geomTemp[j], positions, indices, normals, tex_coords, tex_weights);  
		
		// calculate the centre point of the positions. (size of map 512x512) therefore half for splitting into 4.
		vec3 tl = positions.at(0);
		vec3 tr = positions.at(256);
		vec3 bl = positions.at(positions.size() - 257);
		vec3 br = positions.at(positions.size()-1);

		// push back passed in centres vector.
		centre.push_back((tl + tr + bl + br) * 0.25f);

		// clear vectors for next pass through
		positions.clear();
		normals.clear();
		tex_coords.clear();
		tex_weights.clear();
		indices.clear();
	}

	// add geometry to list.
	for (int i = 0; i < 4; ++i)
	{
		geom.push_back(geomTemp[i]);
	}

	delete data;  // delete data
}

void GenerateBack::generate_bar(geometry &geom)
{
	// method for generating long prisim bar geometery used.

	geom.set_type(GL_TRIANGLES);

	vector<vec3> positions
	{
		vec3(-3.0f, -0.5f, 1.0f),  // first side tri 1
		vec3(2.0f, -0.5f, 1.0f),
		vec3(-2.0f, 0.5f, 0.0f),

		vec3(-2.0f, 0.5f, 0.0f),   // first side tri 2
		vec3(2.0f, -0.5f, 1.0f),
		vec3(2.0f, 0.5f, 0.0f),

		vec3(2.0f, 0.5f, 0.0f),		// first side tri 3
		vec3(2.0f, -0.5f, 1.0f),
		vec3(3.0f, -0.5f, 1.0f),


		vec3(2.0f, 0.5f, 0.0f),  // end on
		vec3(3.0f, -0.5f, 1.0f),
		vec3(3.0f, -0.5f, -1.0f),


		vec3(-2.0f, 0.5f, 0.0f),	// back side tri 1
		vec3(2.0f, -0.5f, -1.0f),
		vec3(-3.0f, -0.5f, -1.0f),

		vec3(2.0f, 0.5f, 0.0f),	 // back side tri 2
		vec3(2.0f, -0.5f, -1.0f),
		vec3(-2.0f, 0.5f, 0.0f),

		vec3(3.0f, -0.5f, -1.0f), // back side tri 3
		vec3(2.0f, -0.5f, -1.0f),
		vec3(2.0f, 0.5f, 0.0f),


		vec3(-3.0f, -0.5f, -1.0f),  // end on
		vec3(-3.0f, -0.5f, 1.0f),
		vec3(-2.0f, 0.5f, 0.0f),

		vec3(3.0, -0.5, 1.0),  		// bottom
		vec3(-3.0, -0.5, 1.0),
		vec3(-3.0, -0.5, -1.0),
		vec3(-3.0, -0.5, -1.0),
		vec3(3.0, -0.5, -1.0),
		vec3(3.0, -0.5, 1.0),

	};

	vector<vec2> tex_coords
	{
		vec2(0.0f, 0.0f),	// front face
		vec2(5.0f, 0.0f),
		vec2(1.0, 1.0f),

		vec2(1.0f, 1.0f),
		vec2(5.0f, 0.0f),
		vec2(5.0f, 1.0f),

		vec2(5.0f, 1.0f),
		vec2(5.0f, 0.0f),
		vec2(6.0f, 0.0f),


		vec2(1.0f, 1.0f),  // end on
		vec2(0.0, 0.0f),
		vec2(2.0f, 0.0f),


		vec2(1.0f, 1.0f), // back side
		vec2(5.0f, 0.0f),
		vec2(0.0f, 0.0f),

		vec2(5.0f, 1.0f),
		vec2(5.0f, 0.0f),
		vec2(1.0f, 1.0f),

		vec2(6.0f, 0.0f),
		vec2(5.0f, 0.0f),
		vec2(5.0f, 1.0f),


		vec2(2.0f, 0.0f),  // end on
		vec2(0.0, 0.0f),
		vec2(1.0f, 1.0f),

		vec2(6.0, 1.0),    // bottom
		vec2(0.0, 1.0),
		vec2(0.0, 0.0),
		vec2(0.0, 0.0),
		vec2(6.0, 0.0),
		vec2(6.0, 1.0),

	};

	vector<vec3> normals;
	
	// calculate normals
	for (unsigned int i = 0; i < positions.size(); i += 3)
	{
		// Calculate normal from position data
		auto v1 = positions[i + 1] - positions[i];
		auto v2 = positions[i + 2] - positions[i];
		auto norm = glm::normalize(glm::cross(v1, v2));
		// Add to normals
		for (unsigned int j = 0; j < 3; ++j)
			normals.push_back(norm);
	}

	// add to buffers
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
}


