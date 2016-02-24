#include "GenerateBack.h"


GenerateBack::GenerateBack()
{
	
}

void GenerateBack::generate_skybox(mesh &skybox, cubemap &cube_map, float skyNum)
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

	// ******************************************************
	// Load the cubemap
	// - create array of six filenames +x, -x, +y, -y, +z, -z
	// ******************************************************
	string folderName;
	if (skyNum == 1)
	{
		folderName = "alien";
	}
	else
		folderName = "new";

	array<string, 6> filenames =
	{
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\posx.png",
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\negx.png",
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\posy.png",
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\negy.png",
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\posz.png",
		"..\\resources\\textures\\cubemaps\\" + folderName + "\\negz.png"
	};

	// ***************
	// Create cube_map
	// ***************
	cube_map = cubemap(filenames);

}

void GenerateBack::generate_terrain(geometry &geom, const texture &height_map, unsigned int width, unsigned int depth, float height_scale)
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

	// ***********************************************************************
	// Part 1 - Iterate through each point, calculate vertex and add to vector
	// ***********************************************************************
	for (int x = 0; x < height_map.get_width(); ++x)
	{
		// *****************************
		// Calculate x position of point
		// *****************************
		float w2 = width / 2;
		point.x = -w2 + (width_point * x);

		for (int z = 0; z < height_map.get_height(); ++z)
		{
			// *****************************
			// Calculate z position of point
			// *****************************
			float d2 = depth / 2;
			point.z = -d2 + (depth_point*z);

			// ****************************************************
			// Y position based on red component of height map data
			// ****************************************************

			point.y = data[(z * height_map.get_width()) + x].y * height_scale;
			// **************************
			// Add point to position data
			// **************************
			positions.push_back(point);
		}
	}

	// ***********************
	// Part 1 - Add index data
	// ***********************
	for (unsigned int x = 0; x < height_map.get_width() - 1; ++x)
	{
		for (unsigned int y = 0; y < height_map.get_height() - 1; ++y)
		{
			// *************************
			// Get four corners of patch
			// *************************
			int top_left = (y * height_map.get_width()) + x;
			int top_right = (y * height_map.get_width()) + x + 1;
			int bottom_left = ((y + 1)*height_map.get_width()) + x;
			int bottom_right = ((y + 1)*height_map.get_height()) + x + 1;

			// ********************************
			// Push back indices for triangle 1
			// ********************************
			indices.push_back(top_left);
			indices.push_back(bottom_right);
			indices.push_back(bottom_left);

			// ********************************
			// Push back indices for triangle 2
			// ********************************
			indices.push_back(top_left);
			indices.push_back(top_right);
			indices.push_back(bottom_right);

		}
	}

	////// GL POLYGON MODE

	// Resize the normals buffer
	normals.resize(positions.size());

	// *********************************************
	// Part 2 - Calculate normals for the height map
	// *********************************************
	for (unsigned int i = 0; i < indices.size() / 3; ++i)
	{
		// ****************************
		// Get indices for the triangle
		// ****************************
		int idx1 = indices[i * 3];
		int idx2 = indices[i * 3 + 1];
		int idx3 = indices[i * 3 + 2];

		// ***********************************
		// Calculate two sides of the triangle
		// ***********************************
		vec3 side1 = positions[idx1] - positions[idx3];
		vec3 side2 = positions[idx1] - positions[idx2];


		// ******************************************
		// Normal is cross product of these two sides
		// ******************************************
		vec3 n = cross(side2, side1);

		// **********************************************************************
		// Add to normals in the normal buffer using the indices for the triangle
		// **********************************************************************
		normals[idx1] += n;
		normals[idx2] += n;
		normals[idx3] += n;

	}

	// *************************
	// Part 2 - Normalize all the normals
	// *************************
	for (auto &n : normals)
	{
		n = normalize(n);
	}

	// *********************************************
	// Part 3 - Add texture coordinates for geometry
	// *********************************************
	for (unsigned int x = 0; x < height_map.get_width(); ++x)
	{
		for (unsigned int z = 0; z < height_map.get_height(); ++z)
		{
			vec2 v;
			v.x = width_point * x;
			v.y = depth_point * z;
			tex_coords.push_back(v);
		}
	}

	// **************************************************
	// Part 4 - Calculate texture weights for each vertex
	// **************************************************
	for (unsigned int x = 0; x < height_map.get_width(); ++x)
	{
		for (unsigned int z = 0; z < height_map.get_height(); ++z)
		{
			// ********************
			// Calculate tex weight
			// ********************
			vec4 w;
			w.x = clamp((1.0 - (length(data[height_map.get_width() * z + x].y) / 0.25)), 0.0, 1.0);
			w.y = clamp((1.0 - (length(data[height_map.get_width() * z + x].y - 0.15) / 0.25)), 0.0, 1.0);
			w.z = clamp((1.0 - (length(data[height_map.get_width() * z + x].y - 0.5) / 0.25)), 0.0, 1.0);
			w.w = clamp((1.0 - (length(data[height_map.get_width() * z + x].y - 0.9) / 0.25)), 0.0, 1.0);

			// ********************************
			// Sum the components of the vector
			// ********************************
			float total = w.x + w.y + w.z + w.w;

			// ********************
			// Divide weight by sum
			// ********************
			w = w / total;

			// *************************
			// Add tex weight to weights
			// *************************
			tex_weights.push_back(w);
		}
	}

	// *************************************
	// Add necessary buffers to the geometry
	// *************************************
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_index_buffer(indices);
	geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	geom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);

	// ***********
	// Delete data
	// ***********
	delete data;
}

