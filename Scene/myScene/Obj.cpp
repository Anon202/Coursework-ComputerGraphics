#include "Obj.h"


Obj::Obj()
{
}

Obj::Obj(vec3 pos, vec3 rot, float theta, vec3 scal,
	mesh* me, material* mate, texture* texture,
	effect* eff,
	directional_light* light)
{
	mat4 T = translate(mat4(1.0f), pos);
	mat4 R = rotate(mat4(1.0f), theta, vec3(1.0, 0.0, 0.0));
	mat4 S = scale(mat4(1.0f), scal);


	mat4 trans = T * (R* S);

	this->mlocal = trans;
	this->m = me;
	this->mat = mate;
	this->tex = texture;
	this->eff = eff;
	this->light = light;
}

void Obj::update(Obj* root, mat4 mparent)
{
	// used to recurse through children and concatenate transforms

	root->mworld = root->mlocal * mparent;

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

void Obj::render(Obj* root)
{
	/*
	 * method to recurse through branch and render all objects
	 */ 


	extern camera* cam;			 // camera pointer 

	// get matrices + eye postion from the camera
	mat4 P = cam->get_projection(); 
	mat4 V = cam->get_view();
	vec3 eyeP = cam->get_position();

	// get normal matrix from mesh
	mat3 N = root->m->get_transform().get_normal_matrix();

	// calculate MVP from world
	auto MVP = P * V * root->mworld;

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
	renderer::bind(root->m->get_material(), "mat");
	renderer::bind(*root->light, "light");
	renderer::bind(*root->tex, 0);
	
	// set texture uniform
	glUniform1i(root->eff->get_uniform_location("tex"), 0);

	// set eye position (from active camera)
	glUniform3f(root->eff->get_uniform_location("eye_pos"), eyeP.x, eyeP.y, eyeP.z);

	// render mesh
	renderer::render(*root->m);

	// recurse for children
	for (auto &e : root->children)
	{
		Obj* child = e.second;
		render(child);
	}

}