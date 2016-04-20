#include "SceneManager.h"
#include "GUI.h"
#include "Obj.h"
#include "Particles.h"
#include "Shadowing.h"



SceneManager::SceneManager(double initialMouseX, double initialMouseY)
{
	// constructor, initialising values.

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
	ssao = false;
	blur = false;
	vig = false;

	// set type of geometry for radii to points.
	radiusGeom.set_type(GL_POINTS);

	fixCull = false; // bool used to turn on/off updating the view frustrum to show culling working

	createLights();

	initialiseParticles(1);
}

void SceneManager::createLights()
{
	directional_light *light = new directional_light;
	light->set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));

	// Light colour white
	light->set_light_colour(vec4(0.9f, 0.79f, 0.55f, 1.0f));

	// Light direction (1.0, 1.0, -1.0)
	light->set_direction(vec3(1.0f, 1.0f, -1.0f));
	Light *lightPtr = light;
	lightList.push_back(lightPtr);
	
	// create pointlights
	point_light *pointLight = new point_light;
	pointLight->set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	pointLight->set_constant_attenuation(0.01f);
	pointLight->set_linear_attenuation(0.05f);
	pointLight->set_quadratic_attenuation(0.001f);
	Light* pointPtr = pointLight;
	lightList.push_back(pointPtr);
	
	// blue point light
	point_light *pointLightChild = new point_light;
	pointLightChild->set_light_colour(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	pointLightChild->set_constant_attenuation(0.01f);
	pointLightChild->set_linear_attenuation(0.05f);
	pointLightChild->set_quadratic_attenuation(0.001f);
	Light* pointPtrChild = pointLightChild;
	lightList.push_back(pointPtrChild);

	// red point light
	point_light *pointLightChildChild = new point_light;
	pointLightChildChild->set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pointLightChildChild->set_constant_attenuation(0.01f);
	pointLightChildChild->set_linear_attenuation(0.05f);
	pointLightChildChild->set_quadratic_attenuation(0.001f);
	Light* ptrChildChild = pointLightChildChild;
	lightList.push_back(ptrChildChild);

	// spot light
	spot_light *spot = new spot_light;
	spot->set_direction(normalize(vec3(-1.0f, 0.0f, 0.0f)));
	spot->set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	spot->set_range(100.0f);
	spot->set_power(0.01f);
	Light* spotPtr = spot;
	lightList.push_back(spotPtr);


}

effect* SceneManager::createEffect(const char vertPath[], const char fragPath[], const char partPath1[], const char partPath2[])
{
	// takes in paths of shaders, builds the effect and adds to a list of pointers.

	effect* myEffect = new effect;								
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
	// method to calculate view frustrum based on camera postion. Called in update function unless fixCull is true.

	//near plane
	float fov = quarter_pi<float>();
	float near = 2.414f;
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

void SceneManager::generateFrustrumPlanes()
{
	// method to regenerate the frustrum geometry from the plane positions, called when culling is fixed.

	vector<vec3> positions
	{
		//near plane
		planePoints[ntl],
		planePoints[nbl],
		planePoints[nbr],
		planePoints[ntr],

		// far plane
		planePoints[ftl],
		planePoints[ftr],
		planePoints[fbr],
		planePoints[fbl],

		// left plane
		planePoints[ftl],
		planePoints[fbl],
		planePoints[nbl],
		planePoints[ntl],

		// right plane 
		planePoints[ntr],
		planePoints[nbr],
		planePoints[fbr],
		planePoints[ftr],

		// top plane
		planePoints[ftl],
		planePoints[ntl],
		planePoints[ntr],
		planePoints[ftr],

		// bottom plane
		planePoints[nbl],
		planePoints[fbl],
		planePoints[fbr],
		planePoints[nbr],
	};

	frustrumGeom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);

}

void SceneManager::initQuad()
{
	screenFrame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	ssaoframe = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	vigFrame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	blurTargetA = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	blurTargetB = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	finalBlur = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());

	vector<vec3> positions
	{
		vec3(1.0f, 1.0f, 0.0f),
		vec3(-1.0f, 1.0f, 0.0f),
		vec3(-1.0f, -1.0f, 0.0f),
		vec3(1.0f, -1.0f, 0.0f)
	};
	vector<vec2> tex_coords
	{
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f)
	};
	screen_quad.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screen_quad.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screen_quad.set_type(GL_QUADS);

	greyEff.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
	greyEff.add_shader("..\\resources\\shaders\\greyscale.frag", GL_FRAGMENT_SHADER);
	greyEff.build();

	blurEff.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
	blurEff.add_shader("..\\resources\\shaders\\blur.frag", GL_FRAGMENT_SHADER);
	blurEff.build();

	simpleTex.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
	simpleTex.add_shader("..\\resources\\shaders\\simple_texture.frag", GL_FRAGMENT_SHADER);
	simpleTex.build();

	ssao_Position.add_shader("..\\resources\\shaders\\ssaoGeomPass.vert", GL_VERTEX_SHADER);
	ssao_Position.add_shader("..\\resources\\shaders\\ssaoGeomPass.frag", GL_FRAGMENT_SHADER);
	ssao_Position.build();

	vignetteEff.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
	vignetteEff.add_shader("..\\resources\\shaders\\vignette.frag", GL_FRAGMENT_SHADER);
	vignetteEff.build();

	bloomEff.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
	bloomEff.add_shader("..\\resources\\shaders\\bloom.frag", GL_FRAGMENT_SHADER);
	bloomEff.build();

	bloomFinalEff.add_shader("..\\resources\\shaders\\simple_texture.vert", GL_VERTEX_SHADER);
	bloomFinalEff.add_shader("..\\resources\\shaders\\bloomFinal.frag", GL_FRAGMENT_SHADER);
	bloomFinalEff.build();
}

void SceneManager::renderVignette()
{
	// renders a vignette on the screen

	// first pass to vignette frame buffer
	renderer::set_render_target(vigFrame);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render scene to frame buffer
	skybx->render();
	transparentObjects.at(0)->renderGlass();
	renderParticles();

	// set target back to screen - 2nd Pass
	renderer::set_render_target();

	renderer::bind(vignetteEff);

	glUniformMatrix4fv(
		vignetteEff.get_uniform_location("MVP"),
		1,
		GL_FALSE,
		value_ptr(mat4(1.0f)));

	renderer::bind(vigFrame.get_frame(), 0);
	glUniform1i(vignetteEff.get_uniform_location("tex"), 0);

	vec2 res = vec2(renderer::get_screen_width(), renderer::get_screen_height());
	glUniform2f(vignetteEff.get_uniform_location("resolution"), res.x, res.y);

	renderer::render(screen_quad);
}

void SceneManager::renderBlur(const bool &bloom)
{
	float screenHeight = (float)renderer::get_screen_height();
	float screenWidth = (float)renderer::get_screen_width();


	double mouseX;
	double mouseY;
	glfwGetCursorPos(renderer::get_window(), &mouseX, &mouseY);

	if (!bloom)
	{
		// render to frame buffer
		renderer::set_render_target(blurTargetA);

		// Clear frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// render scene to frame buffer
		skybx->render();
		transparentObjects.at(0)->renderGlass();
		renderParticles();
	}

	renderer::set_render_target(blurTargetB);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Bind texture shader
	renderer::bind(blurEff);

	// MVP is now the identity matrix
	glUniformMatrix4fv(
		blurEff.get_uniform_location("MVP"), // Location of  uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(mat4(1.0f))); // Pointer to matrix data

	// Bind texture from frame buffer
	renderer::bind(blurTargetA.get_frame(), 0);

	// Set the uniform
	glUniform1i(blurEff.get_uniform_location("tex"), 0);

	float mouseXRatio = (float)mouseX / screenWidth;

	glUniform1f(blurEff.get_uniform_location("radius"), mouseXRatio * 300.0f);

	// horizontal blur
	glUniform2f(blurEff.get_uniform_location("dir"), 1.0f, 0.0f);

	// Render the screen quad
	renderer::render(screen_quad);


	if (!bloom)
	{
		// render to screen
		renderer::set_render_target();
	}
	else
	{
		renderer::set_render_target(finalBlur);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Bind texture shader
	renderer::bind(blurEff);

	// MVP is now the identity matrix
	glUniformMatrix4fv(
		blurEff.get_uniform_location("MVP"), // Location of  uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(mat4(1.0f))); // Pointer to matrix data

	// Bind texture from frame buffer
	renderer::bind(blurTargetB.get_frame(), 0);

	// Set the uniform
	glUniform1i(blurEff.get_uniform_location("tex"), 0);

	float mouseYRatio = (screenHeight - (float)mouseY - 1.0f) / screenHeight;

	glUniform1f(blurEff.get_uniform_location("radius"), mouseYRatio * 300.0f);

	// horizontal blur
	glUniform2f(blurEff.get_uniform_location("dir"), 0.0f, 1.0f);

	glUniform1f(blurEff.get_uniform_location("inverse_width"), (1.0f / screenWidth));
	glUniform1f(blurEff.get_uniform_location("inverse_height"), (1.0f / screenHeight));

	// Render the screen quad
	renderer::render(screen_quad);

}

void SceneManager::renderMyBloom()
{
	// render to frame buffer
	renderer::set_render_target(screenFrame);

	// Clear frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// render scene to frame buffer
	skybx->render();
	transparentObjects.at(0)->renderGlass();
	renderParticles();

	// 2nd pass - render just bright parts
	renderer::set_render_target(blurTargetA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Bind texture shader
	renderer::bind(bloomEff);

	// MVP is now the identity matrix
	glUniformMatrix4fv(
		bloomEff.get_uniform_location("MVP"), // Location of  uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(mat4(1.0f))); // Pointer to matrix data

	// Bind texture from frame buffer
	renderer::bind(screenFrame.get_frame(), 0);

	// Set the uniform
	glUniform1i(bloomEff.get_uniform_location("tex"), 0);

	// Render the screen quad
	renderer::render(screen_quad);

	// blur pass
	renderBlur(true);

	// final render to screen with mix
	renderer::set_render_target();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer::bind(bloomFinalEff);

	glUniformMatrix4fv(
		bloomFinalEff.get_uniform_location("MVP"), // Location of  uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(mat4(1.0f))); // Pointer to matrix data

	// Bind scene texture
	renderer::bind(screenFrame.get_frame(), 0);
	glUniform1i(bloomFinalEff.get_uniform_location("scene"), 0);

	renderer::bind(finalBlur.get_frame(), 1);
	glUniform1i(bloomFinalEff.get_uniform_location("bloomBlur"), 1);
	renderer::render(screen_quad);

}

void SceneManager::renderGreyScale()
{
	// render to frame buffer
	renderer::set_render_target(screenFrame);

	// Clear frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybx->render();  // is sky true (enable/disable depth)
	transparentObjects.at(0)->renderGlass();  // render transparent objects last

	renderParticles();

	renderer::set_render_target();

	// Bind texture shader
	renderer::bind(greyEff);

	// MVP is now the identity matrix
	glUniformMatrix4fv(
		greyEff.get_uniform_location("MVP"), // Location of uniform
		1, // Number of values - 1 mat4
		GL_FALSE, // Transpose the matrix?
		value_ptr(mat4(1.0f))); // Pointer to matrix data

	// Bind texture from frame buffer
	renderer::bind(screenFrame.get_frame(), 0);

	// Set the uniform
	glUniform1i(greyEff.get_uniform_location("tex"), 0);

	// Render the screen quad

	renderer::render(screen_quad);

}

void SceneManager::renderRadii()
{
	if (!debug)
		return;

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	// if debug mode draw radii of bounding spheres
	vector<float> radii;
	vector<vec3> positions;

	// for each object, add it's radius and it's center position in world place to the vectors
	for (auto c : list)
	{
		radii.push_back(c->getRadius());
		vec3 centre = vec3(c->getWorldPos());
		positions.push_back(centre);  // get centre positions
	}

	// add to buffers, dynamic_draw allows for overwriting the buffers.
	radiusGeom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER, GL_DYNAMIC_DRAW);
	radiusGeom.add_buffer(radii, 1, GL_DYNAMIC_DRAW);		// use buffer index 1

	// bind the effect
	renderer::bind(*rad_eff);

	// Calculate ViewProjection matrix, - No model as center positon is already transformed by this

	auto V = cam->get_view();
	auto P = cam->get_projection();
	auto VP = P * V;

	// set uniform
	glUniformMatrix4fv(
		rad_eff->get_uniform_location("VP"),
		1,
		GL_FALSE,
		value_ptr(VP));

	// render the geometry
	renderer::render(radiusGeom);


	// if fixCull, then the frustrum plane is fixed and not updating, so draw if in debug mode
	if (fixCull)
	{
		// generate the geometry from the plane points.
		generateFrustrumPlanes();

		// increase the line width so it's easier to see
		glLineWidth(3.0f);

		renderer::bind(frustrumEffect);

		// set uniform (use view/projection matrix calulated above)
		glUniformMatrix4fv(
			frustrumEffect.get_uniform_location("VP"),
			1,
			GL_FALSE,
			value_ptr(VP));

		renderer::render(getFrustrumGeom());

		glLineWidth(1.0f);
	}

}

void SceneManager::renderScene()
{

	renderRadii(); // render radius of bounding spheres + view frustrum

	renderShadows(); // render shadows

	// render scene depending on what post processing technique is selected
	if (grey)
	{
		renderGreyScale();	// if greyscale render screenquad else render objects normally
	}
	else if (blur)
	{
		renderBlur(false);  // false bool as not bloom blur
	}
	else if (vig)
	{
		renderVignette();
	}
	else if (bloom)
	{
		renderMyBloom();
	}
	else
	{
		skybx->render();  // is sky true (enable/disable depth)
		transparentObjects.at(0)->renderGlass();  // render transparent objects last
		renderParticles();
	}


	// render gui
	if (gui)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderGUI();
	}
}

void SceneManager::updateLightPositions()
{
	// int starting at 1 as directional light is the first light in the list
	for (unsigned int i = 1; i < lightList.size(); ++i)
	{
		lightList[i]->set_position(vec3(lightObjects[i - 1]->getWorldPos()));
	}
}

void SceneManager::updateScene(float delta_time)
{
	updateLightPositions();

	//if (!myScene->getSSAO())

	updateParticles(delta_time);

	// get shadow update
	updateShadows();

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_B))    // need to get an enum for camera tyoe
		bloom = true;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_G))    // need to get an enum for camera tyoe
		bloom = false;

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_1))    // need to get an enum for camera tyoe
		cam = cameraList[0];
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_2))
		cam = cameraList[1];
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_3))
		cam = cameraList[2];

	free_camera* freeCam = NULL;
	freeCam = dynamic_cast<free_camera*>(cam);

	// enable/ disable polygon mode
	if (debug)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (freeCam)
	{
		GLFWwindow* window = renderer::get_window();

		// The ratio of pixels to rotation - remember the fov
		static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
		static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());

		double new_x = 0;
		double new_y = 0;

		glfwGetCursorPos(window, &new_x, &new_y);	// Get the current cursor position

		if (firstMouse)							 // if first mouse take cursor positons from initalised vars
		{
			current_x = initialX; 
			current_y = initialY;
			firstMouse = false;
		}

		double delta_x = 0;
		double delta_y = 0;

		delta_x = new_x - current_x;		 // Calculate delta of cursor positions from last frame
		delta_y = new_y - current_y;


		delta_x *= ratio_width;								 // Multiply deltas by ratios - gets actual change in orientation
		delta_y *= -ratio_height;

		freeCam->rotate((float)delta_x, (float)delta_y);     // Rotate cameras by delta :: delta_y - x-axis rotation :: delta_x - y-axis rotation

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
			freeCam->move(vec3(0.0f, 0.0f, 1.0f)*delta_time*200.0f);
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
			freeCam->move(vec3(-1.0f, 0.0f, 0.0f)*delta_time*200.0f);
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
			freeCam->move(vec3(1.0f, 0.0f, 0.0f)*delta_time*200.0f);
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
			freeCam->move(vec3(0.0f, 0.0f, -1.0f)*delta_time*200.0f);


		glfwGetCursorPos(window, &new_x, &new_y);  // update cursor pos
		current_x = new_x;
		current_y = new_y;
	}

	cam->update(delta_time);  // update the camera

	// FRUSTRUM UPDATE
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_C))
		fixCull = !fixCull;

	if (!fixCull)
	{
		calculateFrustrum();	   // update frustrum
	}

	skybx->update(NULL, delta_time); // null as no parent

	incrementMyTime(2.0f * delta_time); // update myTime for water

	// MOVE SPOTLIGHT
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
		lightObjects[spot - 1]->move(vec3(0.0f, 0.1f, 0.0f));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
		lightObjects[spot - 1]->move(vec3(0.0f, -0.1f, 0.0f));

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT))
		lightObjects[spot - 1]->move(vec3(0.0f, 0.0f, 0.1f));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT))
		lightObjects[spot - 1]->move(vec3(-0.0f, 0.0f, -0.1f));


	static int keystate;
	int newkeystate = glfwGetKey(renderer::get_window(), 'N');

	if (newkeystate && newkeystate != keystate)
	{
		gui = !gui;
	}

	keystate = newkeystate;

	if (gui)
	{
		updateGUI();
	}
	else
	{
		glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

}

void SceneManager::toggleBloom() { bloom = !bloom; }
void SceneManager::toggleDebug() { debug = !debug; }
void SceneManager::toggleVignette() { vig = !vig; }
void SceneManager::toggleBlur() { blur = !blur; }
void SceneManager::toggleGrey() { grey = !grey; }
void SceneManager::toggleCull() { fixCull = !fixCull; }

SceneManager::~SceneManager()
{
	// deconstructor

	// free all memory for lists and clear the pointers

	for (unsigned int i = 0; i < lightList.size(); ++i)
		delete lightList[i];

	lightList.clear();

	for (unsigned int i = 0; i < effectList.size(); ++i)
		delete effectList[i];

	effectList.clear();

	for (unsigned int i = 0; i < cameraList.size(); ++i)
		delete cameraList[i];

	cameraList.clear();

	for (unsigned int i = 0; i < texList.size(); ++i)			// texlist is a vector list of a vector list of texture pointers
	{
		for (unsigned int j = 0; j < texList[i].size(); ++j)     // for each element in texlist delete each element of list inside
			delete texList[i][j]; 
	}
	texList.clear();

	// free memory from gui
	cleanGUI();
}