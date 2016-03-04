#include "main.h"

SceneManager* myScene;  // pointer to a scene manager!

effect frustrumEff;
mesh m;

effect normEffect;
geometry normalGeom;

bool initialise()
{
	double xpos = 0; // create initial vars for mouse position
	double ypos = 0;

	
	// ********************************
	// Set input mode - hide the cursor
	// ********************************
	GLFWwindow* window = renderer::get_window();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ******************************
	// Capture initial mouse position
	// ******************************
	
	glfwGetCursorPos(window, &xpos, &ypos);

	myScene = new SceneManager(xpos, ypos); // pass in (copied in constructor)

	// initialise the cameras and store in pointer list

	// static target camera at pos [0]
	myScene->cam = new target_camera();
	myScene->cameraList.push_back(myScene->cam);  // add to list so as to not loose the pointer to the camera

	// create target camera
	myScene->cam->set_position(vec3(150.0f, 200.0f, -90.0f));
	myScene->cam->set_target(vec3(-1.0f, -1.0f, -1.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	myScene->cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);


	// free_camera!
	myScene->cam = new free_camera();
	myScene->cameraList.push_back(myScene->cam); // add to list (so can be deleted at end)


	// Set camera properties for free camera (default)
	myScene->cam->set_position(vec3(-50.0f, 100.0f, 50.0f));
	myScene->cam->set_target(vec3(0.0f, 0.0f, 1.0f));
	myScene->cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	myScene->Create();  // run create method for scene man

	
	return true;
}



bool load_content()
{
	
	// CREATE TERRAIN
	geometry terrGeom; // geom to load into
	
	// Load height map
	texture height_map("..\\resources\\textures\\heightmaps\\myHeightMapNEW.png");

	// Generate terrain
	myScene->terr->generate_terrain(terrGeom, height_map, 20, 20, 10.0f);

	// create terrain object

	// Use geometry to create terrain mesh
	myScene->meshes["terr"] = mesh(terrGeom);
	
	vector<texture*> terrTextList;				// local list of textures

	terrTextList.push_back( new texture("..\\resources\\textures\\sand.dds"));
	terrTextList.push_back( new texture("..\\resources\\textures\\oranger.jpg"));
	terrTextList.push_back( new texture("..\\resources\\textures\\rock.dds"));
	terrTextList.push_back( new texture("..\\resources\\textures\\snow.dds"));

	myScene->texList.push_back(terrTextList);

	// Create plane mesh
	myScene->meshes["water"] = mesh(geometry_builder::create_plane(200, 200));

	// Create scene
	myScene->meshes["box"] = mesh(geometry_builder::create_box());				// house shape
	myScene->meshes["pyramid"] = mesh(geometry_builder::create_pyramid());

	
	myScene->meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20, vec3(1.0f, 3.0f, 1.0f)));  // pillar

	myScene->meshes["ball"] = mesh(geometry_builder::create_sphere()); // creat ball to emit light
	myScene->materials["ball"].set_diffuse(vec4(1.0, 1.0, 1.0f, 1.0f));

	myScene->meshes["spoot"] = mesh(geometry_builder::create_sphere(20, 20, vec3(5.0f, 5.0f, 5.0f)));
	myScene->meshes["spoot"].get_transform().position = (vec3(-30.5, 200.0, 150.0));
	myScene->materials["spoot"].set_diffuse(vec4(1.0, 1.0, 1.0f, 1.0f));

	// Red box
	myScene->materials["box"].set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));

	// Blue pyramid
	myScene->materials["pyramid"].set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));

	myScene->materials["cylinder"].set_diffuse(vec4(0.53, 0.45, 0.37, 1.0));


	// create platform
	myScene->meshes["platform"] = mesh(geometry_builder::create_box(vec3(8, 0.5, 8)));
	myScene->meshes["platform"].get_geometry().get_maximal_point();
	myScene->materials["platform"].set_diffuse(vec4(0.83, 0.81, 0.68, 1.0));

	myScene->meshes["platBox"] = mesh(geometry_builder::create_box(vec3(1.0, 0.5, 2.0)));
	myScene->materials["platBox"].set_diffuse(vec4(0.83, 0.91, 0.68, 1.0));

	myScene->meshes["platWall"] = mesh(geometry_builder::create_box(vec3(1.5, 3.0, 5.0)));
	myScene->materials["platWall"].set_diffuse(vec4(0.83, 0.71, 0.68, 1.0));


	for (auto &e : myScene->materials)
	{
		e.second.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
		e.second.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		e.second.set_shininess(10.0f);
	}


	// set emissive for point
	myScene->materials["ball"].set_emissive(vec4(1.0f, 1.0f, 0.0f, 1.0f));

	// water needs high spec
	myScene->materials["water"].set_shininess(5.0f);
	myScene->materials["cylinder"].set_shininess(25.0f);

	effect *terr_eff = new effect;
	terr_eff->add_shader("shader.vert", GL_VERTEX_SHADER);
	terr_eff->add_shader("shader.frag", GL_FRAGMENT_SHADER);
	terr_eff->add_shader("..\\resources\\shaders\\parts\\weighted_texture.frag", GL_FRAGMENT_SHADER);
	// Build effect
	terr_eff->build();
	myScene->effectList.push_back(terr_eff);


	myScene->root = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(50.0f, 50.0f, 50.0f), &myScene->meshes["terr"], &myScene->materials["terr"], terrTextList, terr_eff, terrn);
	myScene->root->setName("terrain");

	vector<texture*> objTextList;
	objTextList.push_back(new texture("..\\resources\\textures\\checked.gif"));

	myScene->texList.push_back(objTextList);


	vector<texture*> waterText;
	waterText.push_back(new texture("..\\resources\\textures\\water.jpg"));

	myScene->texList.push_back(waterText);

	vector<texture*> platText;
	platText.push_back(new texture("..\\resources\\textures\\marble.jpg"));
	//platText.push_back(new texture("..\\resources\\textures\\myNMap.png"));
	myScene->texList.push_back(platText);

	vector<texture*> pillarText;
	pillarText.push_back(new texture("..\\resources\\textures\\brick.jpg"));
	pillarText.push_back(new texture("..\\resources\\textures\\brick_normalmap.jpg"));
	myScene->texList.push_back(pillarText);


	effect *water_eff = new effect;
	water_eff->add_shader("..\\resources\\shaders\\water.vert", GL_VERTEX_SHADER);
	water_eff->add_shader("..\\resources\\shaders\\water.frag", GL_FRAGMENT_SHADER);
	water_eff->build();
	myScene->effectList.push_back(water_eff);

	effect *eff = new effect;

	// Load in shaders
	eff->add_shader("..\\resources\\shaders\\phong.vert", GL_VERTEX_SHADER);
	eff->add_shader("..\\resources\\shaders\\phong.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff->build();
	myScene->effectList.push_back(eff);

	effect *norm_eff = new effect;
	norm_eff->add_shader("normShader.vert", GL_VERTEX_SHADER);
	norm_eff->add_shader("normShader.frag", GL_FRAGMENT_SHADER);
	norm_eff->add_shader("..\\resources\\shaders\\parts\\direction.frag", GL_FRAGMENT_SHADER);
	norm_eff->add_shader("..\\resources\\shaders\\parts\\normal_map.frag", GL_FRAGMENT_SHADER);
	norm_eff->build();
	myScene->effectList.push_back(norm_eff);

	
	effect *gouraud_eff = new effect;
	gouraud_eff->add_shader("..\\resources\\shaders\\gouraud.vert", GL_VERTEX_SHADER);
	gouraud_eff->add_shader("..\\resources\\shaders\\gouraud.frag", GL_FRAGMENT_SHADER);
	gouraud_eff->build();
	myScene->effectList.push_back(gouraud_eff);

	effect *shadeff = new effect;
	shadeff->add_shader("..\\resources\\shaders\\shadow.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders
	{
		"shadowShader.frag",
		"..\\resources\\shaders\\parts\\spotPart.frag",
		"..\\resources\\shaders\\parts\\shadowPart.frag"
	};
	shadeff->add_shader(frag_shaders, GL_FRAGMENT_SHADER);
	shadeff->build();
	myScene->effectList.push_back(shadeff);

	

	Obj *pillar = new Obj(vec3(-5.0f, 25.0f, 30.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(0.5f, 0.5f, 0.5f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], pillarText, norm_eff, object);
	Obj *pillar2 = new Obj(vec3(-30.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], pillarText, norm_eff, object);

	Obj *water = new Obj(vec3(0.0f, 5.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(0.1f, 0.1f, 0.1f), &myScene->meshes["water"], &myScene->materials["water"], waterText, water_eff, waterObj);

	Obj *box = new Obj(vec3(30.0f, 15.0f, 60.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["box"], &myScene->materials["box"], objTextList, eff, object);

	Obj *pyra = new Obj(vec3(0.0f, 15.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["pyramid"], &myScene->materials["pyramid"], objTextList, eff, object);

	Obj *ball = new Obj(vec3(30.0f, 35.0f, 60.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(0.05f, 0.05f, 0.05f), &myScene->meshes["ball"], &myScene->materials["ball"], objTextList, eff, pointLightObj);// point_eff, pointLight, pointLightObj);
	

	Obj *plat = new Obj(vec3(-300.0f, 150.0f, 300.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["platform"], &myScene->materials["platform"], platText, eff, object);
	Obj *platBox = new Obj(vec3(160.0, 25.0, 150.0), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["platBox"], &myScene->materials["platBox"], platText, eff, object);

	Obj *platWall = new Obj(vec3(-160.0, 90.0, 100.0), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["platWall"], &myScene->materials["platWall"], platText, eff, object);
	Obj *pillarPlat = new Obj(vec3(-5.0f, 100.0f, 30.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(0.5f, 0.5f, 0.5f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], platText, eff, object);
	Obj *pillarPlat2 = new Obj(vec3(-30.0f, 100.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(0.5f, 0.5f, 0.5f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], platText, eff, object);
	
	
	Obj *spoot = new Obj(vec3(0.0, 0.0, 0.0), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(0.05f, 0.05f, 0.05f), &myScene->meshes["spoot"], &myScene->materials["spoot"], objTextList, eff, object);// point_eff, pointLight, pointLightObj);
	myScene->spot->set_position(myScene->meshes["spoot"].get_transform().position);

	myScene->root->addChild(box, "box");
	myScene->root->addChild(pillar, "pillar");

	myScene->root->addChild(plat, "platform");
	plat->addChild(platWall, "platWall");
	plat->addChild(platBox, "platBox");
	plat->addChild(pillarPlat, "pillarPlat");
	plat->addChild(pillarPlat2, "pillarPlat2");

	pillar->addChild(pillar2, "pillar2");

	myScene->root->addChild(water, "water");
	myScene->root->addChild(spoot, "spoot");

	box->addChild(pyra, "pyramid");

	box->addChild(ball, "ball");



	//int nBufferSize = 0;
	//glBindBuffer(GL_ARRAY_BUFFER, myScene->meshes["box"].get_geometry().get_buffer(BUFFER_INDEXES::POSITION_BUFFER));
	//glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);

	//int originalVertexArraySize = (nBufferSize / sizeof(vec3));

	//vector<vec3> data;

	//glGetBufferSubData(GL_ARRAY_BUFFER, 0, nBufferSize, &data);
	//vec3 cent(0.0, 0.0, 0.0);
	//int largest = 0;
	//for (auto d : data)
	//{
	//	float currentLen = length(d - cent);
	//	if (currentLen > largest)
	//		largest = currentLen;

	//}


	myScene->list.push_back(water);
	myScene->list.push_back(myScene->root);
	myScene->list.push_back(box);
	myScene->list.push_back(pyra);
	myScene->list.push_back(pillar);
	myScene->list.push_back(pillar2);
	myScene->list.push_back(ball);
	myScene->list.push_back(plat);
	myScene->list.push_back(platBox);
	myScene->list.push_back(platWall);
	myScene->list.push_back(pillarPlat);
	myScene->list.push_back(pillarPlat2);
	myScene->list.push_back(spoot);

    // ******************************
    // Create box geometry for skybox
    // ******************************
	myScene->terr->generate_skybox(myScene->meshes["skybox"], myScene->cubemaps["outer"], 1);  // SKY NUMBER ONE
	myScene->terr->generate_skybox(myScene->meshes["skyboxInner"], myScene->cubemaps["inner"], 2);
	
    // *********************
    // Load in skybox effect
    // *********************
	effect *sky_eff = new effect;
	sky_eff->add_shader("..\\resources\\shaders\\skybox.vert", GL_VERTEX_SHADER);
	sky_eff->add_shader("..\\resources\\shaders\\skybox.frag", GL_FRAGMENT_SHADER);

    // Build effect
    sky_eff->build();
	myScene->effectList.push_back(sky_eff);

	myScene->skybx = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(100.0f, 100.0f, 100.0f), &myScene->meshes["skybox"], myScene->cubemaps["outer"], sky_eff);
	myScene->skybx->setName("sky1");
	
	Obj *skybx2 = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f, vec3(100.0f, 100.0f, 100.0f), &myScene->meshes["skyboxInner"], myScene->cubemaps["inner"], sky_eff);

	myScene->list.push_back(skybx2);
	myScene->skybx->addChild(skybx2, "sky2");
	skybx2->addChild(myScene->root, "terrain");  
	myScene->list.push_back(myScene->skybx);

	// create a new shadow effect
	effect *shadow_effect = new effect;
	shadow_effect->add_shader("..\\resources\\shaders\\phong.vert", GL_VERTEX_SHADER);
	shadow_effect->add_shader("..\\resources\\shaders\\phong.frag", GL_FRAGMENT_SHADER);
	shadow_effect->build();
	myScene->shadow_eff = shadow_effect;
	myScene->effectList.push_back(shadow_effect);


	///
	//myScene->radiusGeom;

	effect *rad_eff = new effect;
	rad_eff->add_shader("..\\resources\\shaders\\radiusVert.vert", GL_VERTEX_SHADER);
	rad_eff->add_shader("..\\resources\\shaders\\radiusGeom.geom", GL_GEOMETRY_SHADER);
	rad_eff->add_shader("..\\resources\\shaders\\radiusFrag.frag", GL_FRAGMENT_SHADER);
	rad_eff->build();
	myScene->rad_eff = rad_eff;
	myScene->effectList.push_back(rad_eff);


	frustrumEff.add_shader("..\\resources\\shaders\\gouraud.vert", GL_VERTEX_SHADER);
	frustrumEff.add_shader("..\\resources\\shaders\\gouraud.frag", GL_FRAGMENT_SHADER);
	// Build effect
	frustrumEff.build();


	normEffect.add_shader("..\\resources\\shaders\\normalVert.vert", GL_VERTEX_SHADER);
	normEffect.add_shader("..\\resources\\shaders\\normalGeom.geom", GL_GEOMETRY_SHADER);
	normEffect.add_shader("..\\resources\\shaders\\normalFrag.frag", GL_FRAGMENT_SHADER);
	normEffect.build();
    return true;
}

bool update(float delta_time)
{
	vec3 pos = vec3(0.0, 0.0, 0.0) - myScene->light->get_direction();  /// ???
	myScene->shadow.light_position = myScene->spot->get_position();
	myScene->shadow.light_dir = myScene->spot->get_direction();

	// Press s to save
	if (glfwGetKey(renderer::get_window(), 'Z') == GLFW_PRESS)
	{
		myScene->shadow.buffer->save("test.png");
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_T))    // need to get an enum for camera tyoe
		myScene->cam = myScene->cameraList[0];
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F))
		myScene->cam = myScene->cameraList[1];

	free_camera* freeCam = NULL;
	freeCam = dynamic_cast<free_camera*>(myScene->cam);

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_0))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		myScene->debug = false;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_9))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		myScene->debug = true;
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

		if (myScene->firstMouse)							 // if first mouse take cursor positons from initalised vars
		{
			myScene->current_x = myScene->initialX;
			myScene->current_y = myScene->initialY;
			myScene->firstMouse = false;
		}

		double delta_x = new_x - myScene->current_x;		 // Calculate delta of cursor positions from last frame
		double delta_y = new_y - myScene->current_y;

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


		glfwGetCursorPos(window, &myScene->current_x, &myScene->current_y);  // update cursor pos
	}

	myScene->cam->update(delta_time);  // update the camera
	myScene->calculateFrustrum();	   // calc frustrum
	
	
	myScene->skybx->update(NULL, delta_time); // null as no parent

	
	//myScene->root->update(NULL);
    return true;
}

void generateFrustrumPlanes()
{
	vector<vec3> positions;

	for (int i = 0; i < 8; ++i)
	{
		positions.push_back(myScene->planePoints[i]);  // add all points to vertex buffer  //{ftl 0, ftr 1, fbl 2, fbr 3, ntl 4, ntr 5, nbl 6, nbr 7 };
	}

	vector<GLuint> indices
	{
		// near
		ntl, nbl, nbr,
		nbr, ntr, ntl,

		// far
		ftr, fbr, fbl,
		fbl, ftl, ftr,

		// Right
		ntr, nbr, fbr,
		fbr, ftr, ntr,

		// Left
		ftl, fbl, nbl,
		nbl, ntl, ftl,

		// Top
		nbl, fbl, fbr,
		fbr, nbr, nbl,

		// Bottom
		ftl, ntl, ntr,
		ntr, ftr, ftl


	};

	myScene->frustrumGeom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER, GL_DYNAMIC_DRAW);
	myScene->frustrumGeom.add_index_buffer(indices);

	m = mesh(myScene->frustrumGeom);

}

bool render()
{
	//// render shadow map.
	//renderer::set_render_target(myScene->shadow);

	//// **********************
	//// Clear depth buffer bit
	//// **********************
	//glClear(GL_DEPTH_BUFFER_BIT);

	//// ****************************
	//// Set render mode to cull face
	//// ****************************
	//glCullFace(GL_FRONT);

	//// Bind shader
	//renderer::bind(*myScene->shadow_eff);

	//// Render meshes
	//for (auto &e : myScene->meshes)
	//{
	//	auto m = e.second;
	//	// Create MVP matrix
	//	auto M = m.get_transform().get_transform_matrix();
	//	// *********************************
	//	// View matrix taken from shadow map
	//	// *********************************
	//	auto V = myScene->shadow.get_view();

	//	auto P = myScene->cam->get_projection();
	//	auto MVP = P * V * M;
	//	// Set MVP matrix uniform
	//	glUniformMatrix4fv(
	//		myScene->shadow_eff->get_uniform_location("MVP"), // Location of uniform
	//		1, // Number of values - 1 mat4
	//		GL_FALSE, // Transpose the matrix?
	//		value_ptr(MVP)); // Pointer to matrix data
	//	// Render mesh
	//	renderer::render(m);
	//}

	//// ************************************
	//// Set render target back to the screen
	//// ************************************
	//renderer::set_render_target();

	//// *********************
	//// Set cull face to back
	//// *********************
	//glCullFace(GL_BACK);

	if (myScene->debug)
	{
		// if debug mode draw frustrum?
		vector<float> radii;
		vector<vec3> positions;
		for (auto c : myScene->list)
		{
			radii.push_back(c->getRadius());
			positions.push_back(c->getWorldPos());  // get centre positions
		}

		myScene->radiusGeom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER, GL_DYNAMIC_DRAW);
		myScene->radiusGeom.add_buffer(radii, 1, GL_DYNAMIC_DRAW);


		renderer::bind(*myScene->rad_eff);

		auto V = myScene->cam->get_view();
		auto P = myScene->cam->get_projection();
		auto VP = P * V;

		glUniformMatrix4fv(
			myScene->rad_eff->get_uniform_location("VP"),
			1,
			GL_FALSE,
			value_ptr(VP));


		renderer::render(myScene->radiusGeom);

		generateFrustrumPlanes();

		renderer::bind(frustrumEff);

		auto M = m.get_transform().get_transform_matrix();					// think this is wrong
		auto lV = myScene->cameraList[0]->get_view();
		auto lP = myScene->cameraList[0]->get_projection();

		auto MVP = VP;// lP * lV * M;
		glUniformMatrix4fv(
			frustrumEff.get_uniform_location("MVP"),
			1,
			GL_FALSE,
			value_ptr(MVP));

		renderer::render(m);



		float fov = (0.25f * (float)AI_MATH_PI);
		float far = 1000.f;
		auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());

		vec3 up = normalize(myScene->cameraList[0]->get_up());
		vec3 lookAt;
		float hFar = 2 * tan(fov / 2) * far;		// height of far
		float wFar = hFar * aspect;					// width of far
		lookAt = myScene->cameraList[0]->get_target() - myScene->cameraList[0]->get_position();
		lookAt = normalize(lookAt);
		vec3 right = cross(lookAt, up);					// up cross lookat
		right = normalize(right);
		vec3 farCent = myScene->cameraList[0]->get_position() + (lookAt * far);
		vec3 leftMid = farCent - (right * wFar * 0.5f);

		glBegin(GL_LINES);
		glVertex3f(myScene->cameraList[0]->get_position().x, myScene->cameraList[0]->get_position().y, myScene->cameraList[0]->get_position().z);
		glVertex3f(myScene->cameraList[0]->get_position().x + myScene->planeNormals[bottN].x*30.0f, myScene->cameraList[0]->get_position().y + myScene->planeNormals[bottN].y*30.0f, myScene->cameraList[0]->get_position().z + myScene->planeNormals[bottN].z*30.0f);
		glEnd();

		/*
		// if debug mode draw frustrum
		vector<vec3> normals;
		vector<vec3> normPoints;

		normPoints.push_back(myScene->planePoints[ftl]);
		normPoints.push_back(myScene->planePoints[ntl]);
		normPoints.push_back(myScene->planePoints[nbl]);
		normPoints.push_back(myScene->planePoints[nbr]);
		normPoints.push_back(myScene->planePoints[fbr]);
		normPoints.push_back(myScene->planePoints[ntr]);

		for (auto e : myScene->planeNormals)
		{
			normals.push_back(e);
		}
		normalGeom.add_buffer(normPoints, BUFFER_INDEXES::POSITION_BUFFER, GL_DYNAMIC_DRAW);
		normalGeom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER, GL_DYNAMIC_DRAW);
		


		renderer::bind(normEffect);


		glUniformMatrix4fv(
			normEffect.get_uniform_location("VP"),
			1,
			GL_FALSE,
			value_ptr(VP));


		renderer::render(normalGeom);
		*/
	
	}




	myScene->skybx->render();  // is sky true (enable/disable depth)

	//myScene->root->render(myScene->root);

    return true;
}

void main()
{
    // Create application
    app application;
    // Set methods
    application.set_load_content(load_content);
	application.set_initialise(initialise);
    application.set_update(update);
    application.set_render(render);
    // Run application
    application.run();

	
	for (uint i = 0; i < myScene->list.size(); ++i)          //// getting a memory leak without this regardless of release method...
		delete myScene->list[i];

	myScene->list.clear();

	// method to free memory and delete pointers
	delete myScene;
	myScene = NULL;
}