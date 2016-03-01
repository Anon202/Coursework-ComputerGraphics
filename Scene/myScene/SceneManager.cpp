#include "SceneManager.h"


SceneManager::SceneManager(double initialMouseX, double initialMouseY)
{
	cam = NULL;
	skybx = NULL;
	root = NULL;
	terr = NULL;

	
	// copy vars
	initialX = initialMouseX;
	initialY = initialMouseY;

	current_x = 0;   /// initialise current position of mouse
	current_y = 0;

	firstMouse = false;

	// initialise shadow map with screen size
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());

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
	// method to calculate view frustrum based on camera postion. Recalculated every time camera moves.

	//near plane
	float fov = (0.25f * (float)AI_MATH_PI);
	float near = 0.1f;
	float far = 1000.f;
	float aspect = (renderer::get_screen_width() / renderer::get_screen_height());

	float hNear = 2 * tan(fov / 2) * near;		// height of near
	float wNear = hNear * aspect;				// width of near
	float hFar = 2 * tan(fov / 2) * near;		// height of far
	float wFar = hFar * aspect;					// width of far

	vec3 currentCamPos = cam->get_position();

	vec3 up = normalize(cam->get_up());
	vec3 lookAt = normalize(cam->get_target() - currentCamPos);
	vec3 right = cross(up, lookAt);					// up cross lookat
	right = normalize(right);


	vec3 farCent = currentCamPos + (lookAt * far);		// center point of far plane look at* distance add camera pos
	vec3 nearCent = currentCamPos + (lookAt * near);

	vec3 ftl = farCent + (up * hFar * 0.5f) - (right * wFar * 0.5f);  // far top left - far center + up*half height - right*half width (minus because left)
	vec3 ftr = farCent + (up * hFar * 0.5f) + (right * wFar * 0.5f);  // far top right
	vec3 fbl = farCent - (up * hFar * 0.5f) - (right * wFar * 0.5f);  // far bottom left
	vec3 fbr = farCent - (up * hFar * 0.5f) + (right * wFar * 0.5f);  // far bottom right


	vec3 ntl = nearCent + (up * hNear * 0.5f) - (right * wNear * 0.5f);  // near top left
	vec3 ntr = nearCent + (up * hNear * 0.5f) + (right * wNear * 0.5f);  // near top right
	vec3 nbl = nearCent - (up * hNear * 0.5f) - (right * wNear * 0.5f);  // near bottom left
	vec3 nbr = nearCent - (up * hNear * 0.5f) + (right * wNear * 0.5f);  // near bottom right


	// calculate normals
	planeNormals[0] = cross(nbl - ntl, ftl - ntl);
	planeNormals[1] = cross(fbr - ftr, ntr - ftr);
	planeNormals[2] = cross(ntl - ftl, ftr - ftl);
	planeNormals[3] = cross(fbr - fbl, nbl - fbl);
	planeNormals[4] = lookAt;
	planeNormals[5] = -lookAt;



}


SceneManager::~SceneManager()
{

	//for (uint i = 0; i < list.size(); ++i)
	//	delete list[i];

	//list.clear();

	delete light;
	light = NULL;

	delete pointLight;
	pointLight = NULL;
	
	delete spot;
	spot = NULL;

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