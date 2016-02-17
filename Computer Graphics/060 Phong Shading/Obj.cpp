#include "Obj.h"

Obj::Obj(){}

Obj::Obj(vec3 pos, vec3 rot, float theta, vec3 scal, mesh& me, material& mate, texture& texture) //effect& eff, mat4& PV, vec3& eyeP, directional_light& light)
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
	this->m = &me;
	this->mat = &mate;
	this->tex = &texture;


	/*
	this->eff = &eff;
	this->PV = PV;
	this->eyeP = &eyeP;
	this->light = &light;*/


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

#if 0
void Obj::render( Obj* root ) // effect& eff, mat4& PV, vec3& eyeP, directional_light& light)
{
	// flag
	// return and skip all children



	// continue


	/*
	const int numChildren = root->children.size();
	for (int i = 0; i < numChildren; ++i)
	{
		Obj* child = root->children[i];
		render( child );
	}
	*/
	
	//for (auto &e : root->children)
	//{
		//Obj* child = e.second;
		

		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto MVP = root->PV * root->mworld;
		// Set MVP matrix uniform
		glUniformMatrix4fv(
			root->eff.get_uniform_location("MVP"), // Location of uniform
			1, // Number of values - 1 mat4
			GL_FALSE, // Transpose the matrix?
			value_ptr(MVP)); // Pointer to matrix data

		// ********************
		// Set M matrix uniform
		// ********************
		glUniformMatrix4fv(root->eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(root->mworld));

		// ***********************
		// Set N matrix uniform
		// - remember - 3x3 matrix
		// ***********************
		mat3 N = this->m->get_transform().get_normal_matrix();
		glUniformMatrix3fv(root->eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));


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
		glUniform1i(root->eff.get_uniform_location("tex"), 0);


		// *****************************
		// Set eye position
		// - Get this from active camera
		// *****************************
		glUniform3f(root->eff.get_uniform_location("eye_pos"), root->eyeP.x, root->eyeP.y, root->eyeP.z);


		// Render mesh
		renderer::render(*m);


		//render(child);
	//}

}
#endif