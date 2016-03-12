 #include "SceneManager.h"


SceneManager::SceneManager(double initialMouseX, double initialMouseY)
{
	cam = nullptr;
	skybx = nullptr;
	generator = nullptr;

	
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

	fixCull = false;

}

void SceneManager::createLights()
{
	directional_light *light = new directional_light;
	light->set_ambient_intensity(vec4(0.1f, 0.1f, 0.1f, 1.0f));

	// Light colour white
	light->set_light_colour(vec4(0.9f, 0.79f, 0.55f, 1.0f));

	// Light direction (1.0, 1.0, -1.0)
	light->set_direction(vec3(1.0f, 1.0f, -1.0f));
	Light *lightPtr = light;
	lightList.push_back(lightPtr);
	
	point_light *pointLight = new point_light;
	pointLight->set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	pointLight->set_range(100);
	pointLight->set_constant_attenuation(0.5f);
	pointLight->set_linear_attenuation(0.2f);
	pointLight->set_quadratic_attenuation(0.01f);
	Light* pointPtr = pointLight;
	lightList.push_back(pointPtr);
	

	point_light *pointLightChild = new point_light;
	pointLightChild->set_light_colour(vec4(0.0f, 0.0f,1.0f, 1.0f));
	pointLightChild->set_range(100);
	pointLightChild->set_constant_attenuation(0.5f);
	pointLightChild->set_linear_attenuation(0.2f);
	pointLightChild->set_quadratic_attenuation(0.01f);
	Light* pointPtrChild = pointLightChild;
	lightList.push_back(pointPtrChild);

	spot_light *spot = new spot_light;
	spot->set_direction(normalize(vec3(-1, 0, 0)));
	spot->set_light_colour(vec4(1.0, 1.0, 0.0, 1.0));
	spot->set_range(100);
	spot->set_power(0.01);
	Light* spotPtr = spot;
	lightList.push_back(spotPtr);


}

effect* SceneManager::createEffect(char vertPath[], char fragPath[], char partPath1[], char partPath2[])
{
	effect* myEffect = new effect;								// takes in paths of shaders, builds the effect and adds to a list of pointers.
	myEffect->add_shader(vertPath, GL_VERTEX_SHADER);
	myEffect->add_shader(fragPath, GL_FRAGMENT_SHADER);

	if (partPath1)
		myEffect->add_shader(partPath1, GL_FRAGMENT_SHADER);

	if (partPath2)
		myEffect->add_shader(partPath2, GL_FRAGMENT_SHADER);
	
	
	myEffect->build();
	effectList.push_back(myEffect);
	
	return myEffect;
}

void SceneManager::calculateFrustrum()
{
	//if (fixCull)
	//{
	//if (cam != cameraList[0])
	//		return;
	//}

	//cout << "updatign" << endl;
	// method to calculate view frustrum based on camera postion. Recalculated every time camera moves.

	//near plane
	float fov = quarter_pi<float>();
	float near = 2.414;
	float far = 1000.f;
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());

	float hNear = 2 * tan(fov / 2) * near;		// height of near
	float wNear = hNear * aspect;				// width of near
	float hFar = 2 * tan(fov / 2) * far;		// height of far
	float wFar = hFar * aspect;					// width of far

	vec3 currentCamPos = cam->get_position();

	vec3 lookAt;
	lookAt = normalize(cam->get_target() - cam->get_position());
	vec3 right = cross(lookAt, vec3(0.0f, 1.0f, 0.0f));					// up cross lookat
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


	// Calculate the near and far planes (using camDir and the previously calculated centres)
	planeNormals[nearN] = lookAt;
	planeNormals[farN] = -lookAt;

	// Calculate the left and right planes (cross product to get the normals of the triangles and a point on the planes)
	planeNormals[leftN] = cross(up, (planePoints[fbl] - planePoints[nbl])); 
	planeNormals[rightN] = -cross(up, (planePoints[fbl] - planePoints[nbl]));

	// Calculate the top and bottom planes (similar to the left and right)
	planeNormals[topN] = cross(-right, (planePoints[ntr] - planePoints[ftr]));
	planeNormals[bottN] = cross(-right, (planePoints[fbr] - planePoints[nbr]));

	// normalise normals
	for (int i = 0; i < 6; ++i)
	{
		planeNormals[i] = normalize(planeNormals[i]);
	}

}



SceneManager::~SceneManager()
{

	for (int i = 0; i < lightList.size(); ++i)
		delete lightList[i];

	lightList.clear();

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