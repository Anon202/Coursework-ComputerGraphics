#include "SceneManager.h"


SceneManager::SceneManager(double initialMouseX, double initialMouseY)
{
	cam = nullptr;
	skybx = nullptr;
	root = nullptr;
	terr = nullptr;

	
	// copy vars
	initialX = initialMouseX;
	initialY = initialMouseY;

	current_x = 0;   /// initialise current position of mouse
	current_y = 0;

	firstMouse = false;

	// initialise shadow map with screen size
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());

	debug = false;

	radiusGeom.set_type(GL_POINTS);

}

void SceneManager::Create()
{
	
	light = new directional_light; 
	light->set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));

	// Light colour white
	light->set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Light direction (1.0, 1.0, -1.0)
	light->set_direction(vec3(1.0f, 1.0f, -1.0f));

	
	pointLight = new point_light;
	pointLight->set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	pointLight->set_range(20);
	pointLight->set_position(vec3(31.5, 35.75, 63.0));
	pointLight->set_constant_attenuation(0.5f);
	pointLight->set_linear_attenuation(0.2f);
	pointLight->set_quadratic_attenuation(0.01f);

	spot = new spot_light;
	spot->set_position(vec3(-30.5, 200.0, 150.0));
	spot->set_direction(normalize(vec3(-1, -1, 0)));
	spot->set_light_colour(vec4(1.0, 1.0, 1.0, 1.0));
	spot->set_range(200);
	spot->set_power(50);

}



void SceneManager::calculateFrustrum()
{
	//if (cam != cameraList[0])
	//	return;

	//cout << "updatign" << endl;
	// method to calculate view frustrum based on camera postion. Recalculated every time camera moves.

	//near plane
	float fov = (0.25f * (float)AI_MATH_PI);
	float near = 0.1f;
	float far = 1000.f;
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());

	float hNear = 2 * tan(fov / 2) * near;		// height of near
	float wNear = hNear * aspect;				// width of near
	float hFar = 2 * tan(fov / 2) * far;		// height of far
	float wFar = hFar * aspect;					// width of far

	vec3 currentCamPos = cam->get_position();
	
	vec3 lookAt;
	lookAt = normalize(cam->get_target()-cam->get_position());
	lookAt = normalize(lookAt);
	vec3 right = cross(vec3(0.0f,1.0f,0.0f), lookAt);					// up cross lookat
	right = normalize(right);
	
	vec3 up = normalize(cross(lookAt, right));  //"real up"




	vec3 farCent = currentCamPos + (lookAt * far);		// center point of far plane look at* distance add camera pos
	vec3 nearCent = currentCamPos + (lookAt * near);

	planePoints[ftl] = farCent + (up * hFar * 0.5f) - (right * wFar * 0.5f);  // far top left - far center + up*half height - right*half width (minus because left)
	planePoints[ftr] = farCent + (up * hFar * 0.5f) + (right * wFar * 0.5f);  // far top right
	planePoints[fbl] = farCent - (up * hFar * 0.5f) - (right * wFar * 0.5f);  // far bottom left
	planePoints[fbr] = farCent - (up * hFar * 0.5f) + (right * wFar * 0.5f);  // far bottom right


	planePoints[ntl] = nearCent + (up * hNear * 0.5f) - (right * wNear * 0.5f);  // near top left
	planePoints[ntr] = nearCent + (up * hNear * 0.5f) + (right * wNear * 0.5f);  // near top right
	planePoints[nbl] = nearCent - (up * hNear * 0.5f) - (right * wNear * 0.5f);  // near bottom left
	planePoints[nbr] = nearCent - (up * hNear * 0.5f) + (right * wNear * 0.5f);  // near bottom right


	// calculate normals
	planeNormals[leftN] = cross(planePoints[nbl] - planePoints[ntl], planePoints[ftl] - planePoints[ntl]);
	planeNormals[rightN] = -planeNormals[leftN];//cross(planePoints[fbr] - planePoints[ftr], planePoints[ntr] - planePoints[ftr]);
	
	planeNormals[bottN] = cross(planePoints[nbl] - planePoints[nbr], planePoints[fbl] - planePoints[nbr]);

	planeNormals[topN] = -cross(planePoints[ntl] - planePoints[ntr], planePoints[ftl] - planePoints[ntr]);

	planeNormals[nearN] = lookAt;
	planeNormals[farN] = -lookAt;
	
	// normalise normals
	for (int i = 0; i < 6; ++i)
	{
		planeNormals[i] = normalize(planeNormals[i]);
	}

	
}


SceneManager::~SceneManager()
{

	//for (uint i = 0; i < list.size(); ++i)
	//	delete list[i];

	//list.clear();

	delete light;
	light = nullptr;

	delete pointLight;
	pointLight = nullptr;
	
	delete spot;
	spot = nullptr;

	for (int i = 0; i < effectList.size(); ++i)
		delete effectList[i];

	effectList.clear();

	for (int i = 0; i < cameraList.size(); ++i)
		delete cameraList[i];

	cameraList.clear();

	for (int i = 0; i < texList.size(); ++i)			// texlist is a vector list of a vector list of texture pointers
	{
		for (int j = 0; j < texList[i].size(); ++j)     // for each element in texlist delete each element of list inside
			delete texList[i][j]; 
	}
	texList.clear();
}