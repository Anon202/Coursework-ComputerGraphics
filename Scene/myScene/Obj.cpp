#include "Obj.h"
enum objType { sky, terrain, water, object }; // global enum

Obj::Obj()
{
}

Obj::Obj(vec3 pos, vec3 rot, float theta, vec3 scal,
	mesh* me, material* mate, vector<texture*> texture,
	effect* eff,
	directional_light* light, float myType)
{
	mat4 T = translate(mat4(1.0f), pos);
	mat4 R = rotate(mat4(1.0f), theta, vec3(1.0, 0.0, 0.0));
	mat4 S = scale(mat4(1.0f), scal);


	mat4 trans = T * (R* S);

	this->mlocal = trans;		// copy vars
	this->m = me;
	this->mat = mate;
	this->eff = eff;
	this->light = light;
	this->myType = myType;
	this->tex = texture;

	
}

void Obj::update(Obj* root, mat4 mparent)
{
	// used to recurse through children and concatenate transforms

	//transform by camera positon

	extern SceneManager* myScene;

	camera* cam = myScene->cam;			 // camera pointer 

	if (root->myType == sky)  // if sky box, transform has to move with camera. 
	{
		vec3 difference = cam->get_position() - root->m->get_transform().position;  // get difference in position
		mat4 trans = translate(mat4(1.0f), difference);		
		root->mworld = trans * root->mlocal;										// update transform
	}
	else
	{
		root->mworld = root->mlocal * mparent;
	}

	for (auto &e : root->children)
	{
		Obj* child = e.second;
		update(child, root->mworld);
	}
}

void Obj::addChild(Obj* child, string name)
{
	child->parent = this;
	this->children[name] = child;
}

void Obj::render(Obj* root)//, bool sky)
{
	/*
	 * method to recurse through branch and render all objects
	 */ 
	extern SceneManager* myScene;

    camera* cam = myScene->cam;			 // camera pointer 

	// get matrices + eye postion from the camera
	mat4 P = cam->get_projection(); 
	mat4 V = cam->get_view();
	vec3 eyeP = cam->get_position();

	// get normal matrix from mesh
	mat3 N = root->m->get_transform().get_normal_matrix();

	// calculate MVP from world
	auto MVP = P * V * root->mworld;

	if (root->myType == sky)
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
	}

	// Bind the effect
	renderer::bind(*root->eff);

	// Set MVP matrix uniform
	glUniformMatrix4fv(
		root->eff->get_uniform_location("MVP"), // Location of uniform
		1,									    // Number of values - 1 mat4
		GL_FALSE,							    // Transpose the matrix?
		value_ptr(MVP));						// Pointer to matrix data


	// Set M matrix Uniform
	glUniformMatrix4fv(
		root->eff->get_uniform_location("M"),
		1,
		GL_FALSE,
		value_ptr(root->mworld));

	// Set 3x3 normal matrix from mesh
	glUniformMatrix3fv(
		root->eff->get_uniform_location("N"),
		1,
		GL_FALSE,
		value_ptr(N));

	// Bind Materials/lights/texture
	renderer::bind(*root->mat, "mat");
	renderer::bind(*root->light, "light");
	
	for (int i = 0; i < root->tex.size(); ++i)  // bind every texture from object's list
	{
		renderer::bind(*tex[i], i);
		stringstream stream;
		stream << "tex[" << i << "]";

		glUniform1i(root->eff->get_uniform_location(stream.str()), i);
	}
		//renderer::bind(*root->tex, 0);
	
	// set texture uniform
	//glUniform1i(root->eff->get_uniform_location("tex"), 0);


	// set eye position (from active camera)
	glUniform3f(root->eff->get_uniform_location("eye_pos"), eyeP.x, eyeP.y, eyeP.z);

	// render mesh
	renderer::render(*root->m);

	if (root->myType == sky)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}

	// recurse for children
	for (auto &e : root->children)
	{
		Obj* child = e.second;
		render(child);
	}

}