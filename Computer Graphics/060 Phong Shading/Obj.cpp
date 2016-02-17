#include "Obj.h"

Obj::Obj(){}

Obj::Obj(vec3 pos, vec3 rot, float theta, vec3 scal, mesh* me, material* mate, texture* texture, effect* eff, 
	//const mat4 P, const mat4 V, const vec3 eyeP, 
	directional_light* light)
{

	/*mat4 mlocal;
	mat4 mworld;
	mesh* m;
	material* mat;
	texture* tex;

	vec3 cent;
	float r;

	map<string, Obj*> children;

	bool visible;*/

	mat4 T = translate(mat4(1.0f), pos);
	mat4 R = rotate(mat4(1.0f), theta, vec3(1.0, 0.0, 0.0));
	mat4 S = scale(mat4(1.0f), scal);

	
	mat4 trans = T * (R* S);



	this->mlocal = trans;
	this->m = me;
	this->mat = mate;
	this->tex = texture;


	/*   pointer */
	this->eff = eff;
	//this->myCam = c;
	//this->P = P;
	//this->V = V;
	//this->eyeP = eyeP;
	this->light = light;


}




void Obj::update(Obj* root, mat4 mparent)
{
	// used to concatenate the transforms

	root->mworld = root->mlocal * mparent;


	const int numChildren = root->children.size();

	for (auto &e : root->children)  //int i = 0; i < numChildren; ++i)
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

#if 0
void Obj::render(effect& eff, mat4& PV, vec3& eyeP, directional_light& light)
{
	Obj *root = this;
	renderer::bind(eff);
	// Create MVP matrix
	auto MVP = PV * this->mworld; //root->mworld;
	// Set MVP matrix uniform
	glUniformMatrix4fv(
		eff.get_uniform_location("MVP"), // Location of uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(MVP)); // Pointer to matrix data

	// ********************
	// Set M matrix uniform
	// ********************
	glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(this->mworld));

	// ***********************
	// Set N matrix uniform
	// - remember - 3x3 matrix
	// ***********************
	mat3 N = this->m->get_transform().get_normal_matrix();
	glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));


	// *************
	// Bind material
	// *************
	renderer::bind(this->m->get_material(), "mat");


	// **********
	// Bind light
	// **********
	renderer::bind(light, "light");

	// ************
	// Bind texture
	// ************
	renderer::bind(*tex, 0);


	// ***************
	// Set tex uniform
	// ***************
	glUniform1i(eff.get_uniform_location("tex"), 0);


	// *****************************
	// Set eye position
	// - Get this from active camera
	// *****************************
	glUniform3f(eff.get_uniform_location("eye_pos"), eyeP.x, eyeP.y, eyeP.z);


	// Render mesh
	renderer::render(*m);
}
#endif

#if		1
void Obj::render( Obj* root ) // effect& eff, mat4& PV, vec3& eyeP, directional_light& light)
{
	// flag
	// return and skip all children

	extern camera* cam;

	// continue


	/*
	const int numChildren = root->children.size();
	for (int i = 0; i < numChildren; ++i)
	{
		Obj* child = root->children[i];
		render( child );
	}
	*/

	//root->myCam = cam;
	
	mat4 P = cam->get_projection();
	mat4 V = cam->get_view();
	
	vec3 eyeP = cam->get_position();


	//root->mworld = mat4(1);

	//auto MVP = root->P * root->V * root->mworld;

	auto MVP = P * V * root->mworld;


	//// Bind effect
	renderer::bind(*root->eff);
	// Create MVP matrix
	//auto MVP = root->PV * root->mworld;
	// Set MVP matrix uniform
	glUniformMatrix4fv(
		root->eff->get_uniform_location("MVP"), // Location of uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(MVP)); // Pointer to matrix data

	// ********************
	// Set M matrix uniform
	// ********************
	glUniformMatrix4fv(root->eff->get_uniform_location("M"), 1, GL_FALSE, value_ptr(root->mworld));

	// ***********************
	// Set N matrix uniform
	// - remember - 3x3 matrix
	// ***********************
	mat3 N = root->m->get_transform().get_normal_matrix();
	glUniformMatrix3fv(root->eff->get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));


	// *************
	// Bind material
	// *************
	renderer::bind(root->m->get_material(), "mat");


	// **********
	// Bind light
	// **********
	renderer::bind(*root->light, "light");

	// ************
	// Bind texture
	// ************
	renderer::bind(*root->tex, 0);


	// ***************
	// Set tex uniform
	// ***************
	glUniform1i(root->eff->get_uniform_location("tex"), 0);


	// *****************************
	// Set eye position
	// - Get this from active camera
	// *****************************
	//vec3 eyeP = root->myCam->get_position();
	glUniform3f(root->eff->get_uniform_location("eye_pos"), eyeP.x, eyeP.y, eyeP.z);


	// Render mesh
	renderer::render(*root->m);

	for (auto &e : root->children)
	{
		Obj* child = e.second;
		render(child);
	}

}
#endif