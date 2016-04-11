#include "Obj.h"

Obj::Obj()
{
}

Obj::Obj(vec3 pos, vec3 rot, float theta, vec3 scal,
	mesh* me, material* mate, vector<texture*> texture,
	effect* eff, float myType)
{
	mat4 T = translate(mat4(1.0f), pos);

	mat4 R;

	if (rot == vec3(0.0, 0.0, 0.0))  // if no rotation is given R is just identity matrix
	{
		R = mat4(1.0f);
	}
	else
	{
		R = rotate(mat4(1.0f), theta, rot);
	}

	
	mat4 S = scale(mat4(1.0f), scal);

	mat4 trans = T * (R* S);

	this->mlocal = trans;		// initial local m
	this->m = me;
	this->mat = mate;
	this->eff = eff;
	this->myType = myType;
	this->tex = texture;
	this->theta = theta;
	this->rotV = rot;
	this->normalMatrix = m->get_transform().get_normal_matrix(); // normal matrix is top corner of model matrix (it's orentation)

	visible = true;
	
	calculateSphere(); // calculate bounding sphere
	
	// vars for storing initial translation information
	this->translateMatrix = T;
	this->scaleMatrix = S;
	this->rotationMatrix = R; 
	this->initialTranslation = pos;
}


Obj::Obj(vec3 rot, float theta, vec3 scal,
	mesh* me, cubemap skybox,
	effect* eff)
{
	mat4 R = rotate(mat4(1.0f), theta, rot);
	mat4 S = scale(mat4(1.0f), scal);

	mat4 trans = (R* S);

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

void Obj::setCenterTerr(vec3 cent)
{
	// set centre of terrain object from value

	centreT = cent;

	// furthest point used in bounding sphere calc.
	furthestPoint = centreT;
}


vec4 Obj::getWorldPos()
{
	// method to return world positon of the object as a vector 4

	vec4 pos = vec4(m->get_transform().position, 1.0);

	if (myType == terrn)
	{
		pos = vec4(centreT, 1.0);
	}
	
	pos = mworld * pos; // multiply by model matrix
	
	return pos;

}

float Obj::getRadius()
{
	// retrive scale information from model matrix.
	vec3 scale = vec3(mworld[0].x, mworld[1].y, mworld[2].z);

	// radius is the absolute length of the furthest point in geometry scaled by model matrix scale
	radius = abs(length(scale*furthestPoint));
	
	return radius;
}

void Obj::setTranslationParams(vec3 moveBy, vec3 max)
{
	// sets translation params, bool = true so update function knows to update the matrix otherwise it won't be recalculated.

	translationAdjustment = moveBy;
	maxTranslation = max;

	transUpdate = true;
}

void Obj::setScaleFactor(float sf)
{
	// flag set for updating scale matrix in update function, factor is how much to scale by

	scaleFactor = sf;
	scaleUpdate = true;
}

void Obj::update(Obj* parent, float delta_time)
{
	// used to recurse through children and concatenate transforms

	extern SceneManager* myScene;  // used to get camera transform

	mworld = mlocal;
	

	if (myType == sky)
	{
		mat4 trans = translate(mat4(1.0f), myScene->cam->get_position());

		mat4 rotation = rotate(mat4(1.0f), angleIncrement, rotV);
		angleIncrement += theta * delta_time * 0.01f;   // increment theta over time

		mworld = trans * rotation * mworld;
	}
	else if (myType == movingObject|| myType == pointLightObj) // if theta is not zero, update the rotation + normal matrix
	{
		if (theta != 0.0)
		{
			rotationMatrix = rotate(mat4(1.0f), angleIncrement, rotV);
			normalMatrix = mat3(rotationMatrix);  // change the normal matrix if the local model matrix changes the rotation

			angleIncrement += theta * delta_time;
		}

		if (transUpdate)		// if moveBy value has changed translation matrix needs updating
		{

			// update translation
			totalTranslation += translationAdjustment * delta_time * 0.5f;

			// if the length squared is bigger than the max trans len2 OR smaller than zero, Swap sign (changes direction of translation)
			// length squared is used as sqrt is expensive and not needed for this inequality
			if ((length2(totalTranslation) > length2(maxTranslation)) || length2(totalTranslation) < 0)
				translationAdjustment = -translationAdjustment;

			translateMatrix = translate(mat4(1.0f), initialTranslation + totalTranslation);
		}

		if (scaleUpdate)
		{
			// Update the scale - base on sin wave
			float scaleChange = 1.0f + sinf(myScene->getMyTime());

			// Multiply by object's scale factor
			scaleChange *= scaleFactor;

			vec3 scaleAdjustment = vec3(scaleChange);

			// change object's scale matrix
			scaleMatrix = scale(mat4(1.0f), scaleAdjustment);
		}

		// finally update local model matrix
		mlocal = translateMatrix * rotationMatrix * scaleMatrix;

	}

	// if there is a parent.
	if (parent){
		if (parent->myType != sky && myType != sky)  // Don't want to transform skybox OR take transform from the skybox
		{
			// multiply by parent's model matrix
			mworld = parent->mworld * mlocal;
		}
	}

	intersection();  // intersection testing for view frustrum

	// recurse through all children
	for (auto &e : children)
	{
		Obj* child = e.second;
		child->normalMatrix = normalMatrix;  // set normal matrix for child object (over written when method is recursed over if the child's rotation changes)
		child->update(this, delta_time);
	}

	
}




void Obj::intersection()
{
	extern SceneManager* myScene;

	if (myType == sky)  // always render sky
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
			{
				pointOnPlane = myScene->planePoints[ftl];		// first three planes are far, top and left therefore corner is in all three
			}
			else
			{
				pointOnPlane = myScene->planePoints[nbr];
			}

			vec3 centre = vec3(getWorldPos());

			float d;
			d = dot(myScene->planeNormals[i], centre - pointOnPlane);

			if (d < - getRadius())
			{
				//cout << "CULLING! " << this->myName << endl;
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
	// method used to retrive position data and calculate the largest length of the object	
	// bind array buffer and read subset of data into the vector data to retrieve the positions for the vertices used to calclulate the bounding spheres of the objects
	vector<vec3> data;
	int count = m->get_geometry().get_vertex_count();
	data.resize(count);
	glBindBuffer(GL_ARRAY_BUFFER, m->get_geometry().get_buffer(BUFFER_INDEXES::POSITION_BUFFER));
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)* count, &data[0]);

	float largest = 0.0;
	for (auto p : data)
	{
		float curLen = length(p);		// for every point, caluclate length. 
		if (curLen > largest)			// if length is larger than the current largest length make that the new largest and store the point.
		{
			largest = curLen;
			furthestPoint = p;
		}
	}


}

void Obj::addChild(Obj* child, string name)
{
	child->parent = this;
	child->myName = name;
	this->children[name] = child;
}

void Obj::renderGlass()
{
	if (!visible)  // if the glass is not visible return (don't render)
		return;

	extern SceneManager* myScene;

	camera* cam = myScene->cam;			 // camera pointer 

	// get matrices + eye postion from the camera
	mat4 P = cam->get_projection();
	mat4 V = cam->get_view();
	vec3 eyeP = cam->get_position();

	mat3 N = normalMatrix;  // object has own normalMatrix taken from the rotation from its model matrix

	// calculate MVP from world
	auto MVP = P * V * mworld;

	glEnable(GL_BLEND);		 // enable blend for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	// Bind Materials/lights/texture
	renderer::bind(*mat, "mat");

	vector<point_light> points(3);
	int pointCount = 0;
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
				points.at(pointCount) = *pointLight;  // add multiple point lights
				pointCount++;
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

	renderer::bind(points, "point");  // bind point lights


	for (uint i = 0; i < tex.size(); ++i)  // bind every texture from object's list
	{
		renderer::bind(*tex[i], i);
		stringstream stream;
		stream << "tex[" << i << "]";

		glUniform1i(eff->get_uniform_location(stream.str()), i);
	}

	// set eye position (from active camera)
	glUniform3f(eff->get_uniform_location("eye_pos"), eyeP.x, eyeP.y, eyeP.z);

	float transparencyValue = 0.3f;
	glUniform1f(eff->get_uniform_location("alphaVal"), transparencyValue); // glass object so true

	// render mesh
	renderer::render(*m);

	//glDisable(GL_BLEND);									 // enable blend for transparency
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Obj::render()
{
	extern SceneManager* myScene;
	/*
	 * method to recurse through branch and render all objects
	 */ 




	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_BLEND);
	if (visible && myType != glassOb)
	{


		camera* cam = myScene->cam;			 // camera pointer 

		float transparencyValue = 1.0f;

		// get matrices + eye postion from the camera
		mat4 P = cam->get_projection();
		mat4 V = cam->get_view();
		vec3 eyeP = cam->get_position();

		mat3 N = normalMatrix;  // object has own normalMatrix taken from the rotation from its model matrix

		// calculate MVP from world
		auto MVP = P * V * mworld;

		if (myType == sky)
		{
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
		}
		else
		{

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


		// uniform for float - used by water and point light to displace vertexes
		//incremented in update function
		glUniform1f(eff->get_uniform_location("myTime"), myScene->getMyTime());
	
		if (myType == waterObj)  // water flag to assign uniform moving water!
		{

			transparencyValue = 0.7f;
		}
	
		// Bind Materials/lights/texture
		renderer::bind(*mat, "mat");

		vector<point_light> points(3);
		int pointCount = 0;
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
					//renderer::bind(*pointLight, "point[2]");
					points.at(pointCount) = *pointLight;
					pointCount++;
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
		
		if (myType == forShade)
		{
			renderer::bind(myScene->shadow.buffer->get_depth(), 6);
			glUniform1i(eff->get_uniform_location("shadow_map"), 6);

			auto lV = myScene->shadow.get_view();

			auto lMVP = P * lV * mworld;


			glUniformMatrix4fv(
				eff->get_uniform_location("lightMVP"),
				1,
				GL_FALSE,
				value_ptr(lMVP));
		}


		renderer::bind(points, "point");

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
		glUniform3fv(eff->get_uniform_location("eye_pos"), 1, value_ptr(cam->get_position()));


		// all objects in this loop are opaque objects therefore alpha is 1
		glUniform1f(eff->get_uniform_location("alphaVal"), transparencyValue); 



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