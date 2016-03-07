#include "Obj.h"

Obj::Obj()
{
}

Obj::Obj(vec3 pos, vec3 rot, float theta, vec3 scal,
	mesh* me, material* mate, vector<texture*> texture,
	effect* eff, float myType)
{
	mat4 T = translate(mat4(1.0f), pos);
	if (myType == spotty || myType == pointLightObj)
		T = translate(mat4(1.0f), me->get_transform().position);
	mat4 R = rotate(mat4(1.0f), theta, rot);
	mat4 S = scale(mat4(1.0f), scal);


	mat4 trans = T * (R* S);

	this->mlocal = trans;		// copy vars
	this->m = me;
	this->mat = mate;
	this->eff = eff;
	this->myType = myType;
	this->tex = texture;
	this->theta = theta;
	this->rotV = rot;

	visible = true;
	
	calculateSphere(); // calculate bounding sphere
	
}


Obj::Obj(vec3 pos, vec3 rot, float theta, vec3 scal,
	mesh* me, cubemap skybox,
	effect* eff)
{
	mat4 T = translate(mat4(1.0f), pos);
	if (myType == spotty)
		T = translate(mat4(1.0f), me->get_transform().position);
	mat4 R = rotate(mat4(1.0f), theta, rot);
	mat4 S = scale(mat4(1.0f), scal);


	mat4 trans = T * (R* S);

	this->mlocal = trans;		// copy vars
	this->m = me;
	this->eff = eff;
	this->myType = sky;
	this->myCubemap = skybox;
	this->theta = theta;
	this->rotV = rot;

	visible = true;

	calculateSphere(); // calculate bounding sphere

}


void Obj::setName(string name)
{
	myName = name;
}

vec4 Obj::getWorldPos()
{
	vec4 pos = mworld * vec4(m->get_transform().position, 1.0);

	return pos;

}

float Obj::getRadius()
{
	vec3 scale = vec3(mworld[0].x, mworld[1].y, mworld[2].z);

	radius = abs(length(scale*furthestPoint));

	return radius;
}

void Obj::update(Obj* parent, float delta_time)
{
	// used to recurse through children and concatenate transforms

	//transform by camera positon

	extern SceneManager* myScene;


	mworld = mlocal;

	if (myType == sky)
	{
		//vec3 difference = cam->get_position();// -m->get_transform().position;  // get difference in position
		mat4 trans = translate(mat4(1.0f), myScene->cam->get_position());

		mat4 rotation = rotate(mat4(1.0f), theta, rotV);
		if (myName == "skyInner")
			theta += pi<float>() * delta_time * 0.01f;   // increment theta over time

		mworld = trans * rotation * mworld;
	}

	//if (myName == "")
	//{
	//	mat4 trans = translate(mat4(1.0f), myScene->cam->get_position());

	//	mat4 rotation = rotate(mat4(1.0f), theta, rotV);
	//	theta += pi<float>() * delta_time * 0.01f;   // increment theta over time

	//	mworld = trans * rotation * mworld;
	//}


	if (parent){
		if (parent->myType != sky && myType != sky)
		{
			mworld *= parent->mworld;
		}
	}

	intersection();

	for (auto &e : children)
	{
		Obj* child = e.second;
		child->update(this, delta_time);
	}

	
}



void Obj::intersection()
{
	extern SceneManager* myScene;
	
	
	
	// ** CLIP SPACE APPROACH  ** //
	// Point in world position, (M * Position)
	// point in clip space = P * V * worldPos(vector4 w =1)
	// "normalised" w needs to be = 1 therefore... all components are divided by w.
	// pointClipNormalised = point in clip space / w component
	//vec4 pointClip = myScene->cam->get_projection() * myScene->cam->get_view() * getWorldPos();
	//vec3 pointClipNormalised = vec3(pointClip.x / pointClip.w, pointClip.y / pointClip.w, pointClip.z / pointClip.w);


	if (myType == sky || myType == terrn)  // always render terrain and sky
	{
		visible = true;
	}
	else if (parent->visible)  // check if parent is visible if not don't bother and set child as false
	{
		visible = true;

		for (int i = 0; i < myScene->planeNormals->length(); ++i) // for each plane check if intersection occurs
		{
		
			vec3 pointOnPlane;

			if (i < 3)
				pointOnPlane = myScene->planePoints[ftl];
			else
				pointOnPlane = myScene->planePoints[nbr];

			vec3 centre = vec3(getWorldPos());

			float d;
			d = dot(myScene->planeNormals[i], centre - pointOnPlane);

			if (d <= -radius)
			{
				cout << "CULLING! " << this->myName << endl;
				visible = false;
				break;
			}

		}
	}
	else // if parent is not visible child should not be visible
	{
		visible = false;
	}

}

void Obj::calculateSphere()
{
	// need to calculate bounding sphere for the object
	
	vector<vec3> data;
	int count = m->get_geometry().get_vertex_count();
	data.resize(count);
	glBindBuffer(GL_ARRAY_BUFFER, m->get_geometry().get_buffer(BUFFER_INDEXES::POSITION_BUFFER));
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)* count, &data[0]);

	float largest = 0.0;
	for (auto p : data)
	{
		float curLen = length(p);
		if (curLen > largest)
		{
			largest = curLen;
			furthestPoint = p;
		}
	}

	radius = largest;


}

void Obj::addChild(Obj* child, string name)
{
	child->parent = this;
	child->myName = name;
	this->children[name] = child;
}

void Obj::render()
{
	/*
	 * method to recurse through branch and render all objects
	 */ 
	if (visible)
	{
		extern SceneManager* myScene;

		camera* cam = myScene->cam;			 // camera pointer 

		// get matrices + eye postion from the camera
		mat4 P = cam->get_projection();
		mat4 V = cam->get_view();
		vec3 eyeP = cam->get_position();


		// get normal matrix from mesh
		mat3 N = m->get_transform().get_normal_matrix();

		// calculate MVP from world
		auto MVP = P * V * mworld;

		if (myType == sky)
		{
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
		}

		// Bind the effect
		renderer::bind(*eff);

		// Set MVP matrix uniform
		glUniformMatrix4fv(
			eff->get_uniform_location("MVP"), // Location of uniform
			1,									    // Number of values - 1 mat4
			GL_FALSE,							    // Transpose the matrix?
			value_ptr(MVP));						// Pointer to matrix data


		// Set M matrix Uniform
		glUniformMatrix4fv(
			eff->get_uniform_location("M"),
			1,
			GL_FALSE,
			value_ptr(mworld));

		// Set 3x3 normal matrix from mesh
		glUniformMatrix3fv(
			eff->get_uniform_location("N"),
			1,
			GL_FALSE,
			value_ptr(N));


		//auto T = glm::translate(mat4(1.0f), myScene->spot->get_position());
		//auto R = glm::mat4_cast(glm::quat(myScene->spot->get_direction()));
		//auto matrix = T * R;
		//auto lV = myScene->shadow.get_view();

		//auto lMVP = P * lV * matrix;

		//if (myType == forShade)
		//{
		//	glUniformMatrix4fv(
		//		eff->get_uniform_location("lightMVP"),
		//		1,
		//		GL_FALSE,
		//		value_ptr(lMVP));
		//}

		if (waterObj)  // water flag to assign uniform moving water!
		{
			static float dd = 0.0f;
			dd += 0.002f;
			glUniform1f(eff->get_uniform_location("myTime"), dd);
		}

		// Bind Materials/lights/texture
		renderer::bind(*mat, "mat");


		// cast light to correct type to call render bind
		for (auto &e : myScene->lightList)
		{
			directional_light *myLight = dynamic_cast<directional_light*>(e);
			if (myLight != NULL)
			{
				renderer::bind(*myLight, "light");
			}
			else 
			{
				point_light *pointLight = dynamic_cast<point_light*>(e);
				if (pointLight != NULL)
				{
					renderer::bind(*pointLight, "point");
				}
				else
				{

					spot_light *spotLight = dynamic_cast<spot_light*>(e);
					if (spotLight != NULL)
					{
						renderer::bind(*spotLight, "spot");
					}
				}
			}
		}

		renderer::bind(myScene->shadow.buffer->get_depth(), 1);


		if (myType == sky)
		{
			renderer::bind(myCubemap, 0);
			glUniform1i(eff->get_uniform_location("cubemap"), 0);
			
			bool trans = false;
			if (myName == "skyInner")
				trans = true;

			
			glUniform1f(eff->get_uniform_location("transparency"), trans);
		}
		else
		{

			for (uint i = 0; i < tex.size(); ++i)  // bind every texture from object's list
			{
				renderer::bind(*tex[i], i);
				stringstream stream;
				stream << "tex[" << i << "]";

				glUniform1i(eff->get_uniform_location(stream.str()), i);
			}
		}

		// set eye position (from active camera)
		glUniform3f(eff->get_uniform_location("eye_pos"), eyeP.x, eyeP.y, eyeP.z);




		// render mesh
		renderer::render(*m);

		

		if (myType == sky)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
		}

		// recurse for children
		for (auto &e : children)
		{
			Obj* child = e.second;
			child->render();
		}
	}


}