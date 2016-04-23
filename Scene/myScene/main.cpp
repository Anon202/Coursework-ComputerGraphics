#include "main.h"

SceneManager* myScene;  // pointer to a scene manager!

bool initialise()
{
	double xpos = 0; // create initial vars for mouse position
	double ypos = 0;

	
	// Set input mode - hide the cursor
	GLFWwindow* window = renderer::get_window();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Capture initial mouse position	
	glfwGetCursorPos(window, &xpos, &ypos);

	myScene = new SceneManager(xpos, ypos); // pass in (copied in constructor)

	// initialise the cameras and store in pointer list

	// static target camera at pos [0]
	myScene->cam = new target_camera();
	

	// create target camera camera at point lights
	myScene->cam->set_position(vec3(240.0, 150.0, -200.0));
	myScene->cam->set_target((vec3(240.0, 90.0, -310.0)));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	myScene->cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
	myScene->cameraList.push_back(myScene->cam);  // add to list so as to not loose the pointer to the camera

	// target camera 2
	myScene->cam = new target_camera();
	myScene->cameraList.push_back(myScene->cam); // add to list (so can be deleted at end)


	// Set camera properties for 2nd target camera
	myScene->cam->set_position(vec3(-260.0f, 200.0f, 0.0f));
	myScene->cam->set_target(vec3(-261.0f, 200.0f, 10.0f));
	myScene->cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);


	// free_camera!
	myScene->cam = new free_camera();
	myScene->cameraList.push_back(myScene->cam); // add to list (so can be deleted at end)


	// Set camera properties for free camera (default)
	myScene->cam->set_position(vec3(-50.0f, 100.0f, 50.0f));
	myScene->cam->set_target(vec3(0.0f, 0.0f, 1.0f));
	myScene->cam->set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	//initialise gui
	initialiseGUI(window);

	return true;
}

bool load_content()
{
	myScene->shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());
	// CREATE TERRAIN

	vector<geometry> terrainBlocks;	// geom to pass into function to create terrain blocks within
	vector<vec3> centres;			// list of terrain centers calculated from the positons when generating terrain
	
	// Load height map
	texture height_map("..\\resources\\textures\\heightmaps\\myHeightMapNEW.png");

	// Generate terrain
	myScene->generator->generate_terrain(terrainBlocks, height_map, 20, 20, 10.0f, centres);

	// creates terrain object
	// Use geometry to create terrain meshes
	myScene->meshes["terr"] = mesh(terrainBlocks.at(0));
	myScene->meshes["terr2"] = mesh(terrainBlocks.at(1));
	myScene->meshes["terr3"] = mesh(terrainBlocks.at(2));
	myScene->meshes["terr4"] = mesh(terrainBlocks.at(3));
	
	vector<texture*> terrTextList;				// local list of textures
	terrTextList.push_back( new texture("..\\resources\\textures\\Big_pebbles_pxr128.tif"));
	terrTextList.push_back( new texture("..\\resources\\textures\\Peetmoss_pxr128.tif"));
	terrTextList.push_back( new texture("..\\resources\\textures\\Jute_cocomat_pxr128.tif"));
	terrTextList.push_back( new texture("..\\resources\\textures\\Slag_stone_pxr128.tif"));
	myScene->texList.push_back(terrTextList);	// add to list of texture lists.

	// Create plane mesh for the water 
	myScene->meshes["water"] = mesh(geometry_builder::create_plane(200, 200));

	// Create scene
	myScene->meshes["box"] = mesh(geometry_builder::create_box());				// house shape
	myScene->meshes["pyramid"] = mesh(geometry_builder::create_pyramid());

	myScene->meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20, vec3(1.0f, 3.0f, 1.0f)));  // pillar

	myScene->meshes["pointLightParent"] = mesh(geometry_builder::create_sphere(50, 50)); // create ball to emit light (higher stacks as using vertex displacement on these)

	myScene->meshes["spotlight"] = mesh(geometry_builder::create_sphere(20, 20, vec3(0.1f, 0.1f, 0.1f)));
	myScene->materials["spotlight"].set_diffuse(vec4(1.0, 1.0, 1.0f, 1.0f));

	// Red box
	myScene->materials["box"].set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));

	// Blue pyramid
	myScene->materials["pyramid"].set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));

	myScene->materials["cylinder"].set_diffuse(vec4(0.53, 0.45, 0.37, 1.0));


	// create platform geometry and set diffuse materials
	myScene->meshes["platform"] = mesh(geometry_builder::create_box(vec3(8, 0.25, 8)));
	myScene->meshes["platform"].get_geometry().get_maximal_point();
	myScene->materials["platform"].set_diffuse(vec4(0.83, 0.81, 0.68, 1.0));

	myScene->meshes["platBox"] = mesh(geometry_builder::create_box(vec3(0.5, 2.0, 3.0)));
	myScene->materials["platBox"].set_diffuse(vec4(0.83, 0.81, 0.68, 1.0));

	myScene->meshes["platWall"] = mesh(geometry_builder::create_box(vec3(0.5, 3.0, 5.0)));
	myScene->materials["platWall"].set_diffuse(vec4(0.83, 0.71, 0.68, 1.0));

	myScene->meshes["model"] = geometry("..\\resources\\models\\stoneEnd.3ds");
	myScene->materials["model"].set_diffuse(vec4(0.83, 0.71, 0.68, 1.0));

	myScene->meshes["glass"] = mesh(geometry_builder::create_box(vec3(0.1, 2.0, 1.0)));// mesh(glassP);

	myScene->materials["glass"].set_diffuse(vec4(1.0, 1.0, 1.0, 1.0));


	// create torus geom for difference between gouraud and phong
	myScene->meshes["sphere"] = mesh(geometry_builder::create_sphere(15, 15));
	myScene->materials["sphere"].set_diffuse(vec4(1.0, 0.0, 1.0, 1.0));

	// create geometry from generator of bar for tops of pillars
	geometry barGeom;
	myScene->generator->generate_bar(barGeom);
	myScene->meshes["bar"] = barGeom;

	// set standard values for all materials
	for (auto &e : myScene->materials)
	{
		e.second.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
		e.second.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		e.second.set_shininess(5.0f);
	}

	// set emissive for spot
	myScene->materials["spotlight"].set_emissive(vec4(1.0f, 1.0f, 0.0f, 1.0f));

	// water needs high spec
	myScene->materials["water"].set_shininess(50.0f);
	myScene->materials["cylinder"].set_shininess(50.0f);


	myScene->materials["glass"].set_shininess(50.0f);

	// high spec for gouraud/phong sphere to illustrate difference - phong will be more concentrated
	myScene->materials["sphere"].set_shininess(50.0f);

	// low spec for model as rock
	myScene->materials["model"].set_specular(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	
	// set emissive for points, diffuse to alter texture col, specular is black
	myScene->materials["pointLightYellow"].set_diffuse(vec4(0.8, 0.8, 0.0f, 1.0f));
	myScene->materials["pointLightYellow"].set_emissive(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	myScene->materials["pointLightYellow"].set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	myScene->materials["pointLightBlue"].set_emissive(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	myScene->materials["pointLightBlue"].set_diffuse(vec4(0.0, 0.0, 1.0f, 1.0f));
	myScene->materials["pointLightBlue"].set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	myScene->materials["pointLightRed"].set_emissive(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	myScene->materials["pointLightRed"].set_diffuse(vec4(0.6, 0.0, 1.0f, 1.0f));
	myScene->materials["pointLightRed"].set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	effect *terr_eff = myScene->createEffect(
		"..\\resources\\shaders\\terrainShader.vert",
		"..\\resources\\shaders\\terrainShader.frag",
		"..\\resources\\shaders\\parts\\point.frag",
		"..\\resources\\shaders\\parts\\weighted_texture.frag");

	effect *phongEff = myScene->createEffect(
		"..\\resources\\shaders\\phong.vert",
		"..\\resources\\shaders\\phong.frag",
		"..\\resources\\shaders\\parts\\point.frag", NULL);


	// Terrain: 1 2
	//			3 4
	// looking at platform
	Obj *terrain1 = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(50.0f, 50.0f, 50.0f), &myScene->meshes["terr3"], &myScene->materials["terr"], terrTextList, terr_eff, terrn);
	Obj *terrain2 = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(50.0f, 50.0f, 50.0f), &myScene->meshes["terr"], &myScene->materials["terr"], terrTextList, terr_eff, terrn);
	Obj *terrain3 = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(50.0f, 50.0f, 50.0f), &myScene->meshes["terr2"], &myScene->materials["terr"], terrTextList, terr_eff, terrn);
	Obj *terrain4 = new Obj(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(50.0f, 50.0f, 50.0f), &myScene->meshes["terr4"], &myScene->materials["terr"], terrTextList, terr_eff, terrn);
	
	
	myScene->list.push_back(terrain2);
	myScene->list.push_back(terrain3);
	myScene->list.push_back(terrain1);
	myScene->list.push_back(terrain4);

	for (int i = 0; i < 4; ++i)
	{
		myScene->list.at(i)->setCenterTerr(centres[i]);
	}

	// create texture lists for objects and add them to the scene manager's list

	vector<texture*> sphereText;
	sphereText.push_back(new texture("..\\resources\\textures\\Red_velvet_pxr128.tif"));
	myScene->texList.push_back(sphereText);

	vector<texture*> woodenTextures;
	woodenTextures.push_back(new texture("..\\resources\\textures\\Vertical_redwood_pxr128.tif"));
	woodenTextures.push_back(new texture("..\\resources\\textures\\Vertical_redwood_pxr128_normal.tif"));
	myScene->texList.push_back(woodenTextures);


	vector<texture*> glassText;
	glassText.push_back(new texture("..\\resources\\textures\\blending_transparent_window.png"));
	myScene->texList.push_back(glassText);

	vector<texture*> waterText;
	waterText.push_back(new texture("..\\resources\\textures\\water.jpg"));

	myScene->texList.push_back(waterText);

	vector<texture*> platText;
	platText.push_back(new texture("..\\resources\\textures\\Gray_swirl_marble_pxr128.tif"));
	platText.push_back(new texture("..\\resources\\textures\\Gray_swirl_marble_pxr128_normal.tif"));
	platText.push_back(new texture("..\\resources\\textures\\Ivy_pxr128.tif"));
	platText.push_back(new texture("..\\resources\\textures\\Ivy_pxr128_normal.tif"));
	platText.push_back(new texture("..\\resources\\textures\\dissolve.png"));
	myScene->texList.push_back(platText);

	vector<texture*> pillarText;
	pillarText.push_back(new texture("..\\resources\\textures\\brick.jpg"));
	pillarText.push_back(new texture("..\\resources\\textures\\brick_normalmap.jpg"));
	myScene->texList.push_back(pillarText);

	vector<texture*> stoneModText;
	stoneModText.push_back(new texture("..\\resources\\textures\\stoneEnd_D.tga"));
	stoneModText.push_back(new texture("..\\resources\\textures\\stoneEnd_N.tga"));
	myScene->texList.push_back(stoneModText);

	vector<texture*> displacementTextures;
	displacementTextures.push_back(new texture("..\\resources\\textures\\Iridescent_ribbon_pxr128.tif"));
	displacementTextures.push_back(new texture("..\\resources\\textures\\dispTest.png"));
	myScene->texList.push_back(displacementTextures);

	effect *water_eff = myScene->createEffect(
		"..\\resources\\shaders\\water.vert",
		"..\\resources\\shaders\\phong.frag",
		"..\\resources\\shaders\\parts\\point.frag",
		NULL);

	effect *norm_eff = myScene->createEffect(
		"..\\resources\\shaders\\normShader.vert",
		"..\\resources\\shaders\\normShader.frag",
		"..\\resources\\shaders\\parts\\direction.frag",
		"..\\resources\\shaders\\parts\\normal_map.frag");
	
	effect *gouraud_eff = myScene->createEffect(
		"..\\resources\\shaders\\gouraud.vert",
		"..\\resources\\shaders\\gouraud.frag",
		NULL, NULL);

	effect *blending = myScene->createEffect(
		"..\\resources\\shaders\\normShader.vert",
		"..\\resources\\shaders\\blending.frag",
		"..\\resources\\shaders\\parts\\normal_map.frag", NULL);

	effect *displacement = myScene->createEffect(
		"..\\resources\\shaders\\displacement.vert",
		"..\\resources\\shaders\\displacement.frag",
		"..\\resources\\shaders\\parts\\point.frag",
		NULL);


	// add shadowEffect
	effect *shadowEff = myScene->createEffect(
		"..\\resources\\shaders\\shadow.vert",
		"..\\resources\\shaders\\shadowShader.frag",
		"..\\resources\\shaders\\parts\\spotPart.frag",
		"..\\resources\\shaders\\parts\\shadowPart.frag");

	/*	Create object pointers for scene, constructor parameters are translation, rotation axis, rotation angle, scale, mesh, material, texturelist, effect, and enum
	 *	enum to store as the object's type used for conditional statements for example a "glassObject" which is treated differently to a normal "object"
	 */

	// create objects to show difference between gouraud and phong shading
	Obj *sphereG = new Obj(vec3(5.0f, 3.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(0.5), &myScene->meshes["sphere"], &myScene->materials["sphere"], sphereText, gouraud_eff, movingObject);
	Obj *sphereP = new Obj(vec3(0.0, 0.0f, -2.0), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(1.0), &myScene->meshes["sphere"], &myScene->materials["sphere"], sphereText, phongEff, object);
	
	// translation transformation, takes in increment value (slower the lower) and max distance to move
	sphereG->setTranslationParams(vec3(0.0f, 0.75f, -0.75f), vec3(0.0f, 1.5f, 1.5f));

	// create objects for the "temple" - platform is the root of this
	Obj *platLower = new Obj(vec3(-6.0f, 2.8f, 6.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["platform"], &myScene->materials["platform"], platText, shadowEff, object);
	Obj *platformUpper = new Obj(vec3(0.0f, 0.2f, 0.0f), vec3(0.0f), 0.0f, vec3(0.9f), &myScene->meshes["platform"], &myScene->materials["platform"], platText, shadowEff, object);

	// pillars underneath platform 
	Obj *pillar = new Obj(vec3(3.0f, -1.5f, -3.5f), vec3(0.0f), 0.0f, vec3(0.5f, 1.0f, 0.5f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], pillarText, norm_eff, object);
	Obj *pillar2 = new Obj(vec3(-1.0f, -1.5f, -3.5f), vec3(0.0f), 0.0f, vec3(0.5f, 1.0f, 0.5f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], pillarText, norm_eff, object);
	Obj *pillar3 = new Obj(vec3(1.0f, -1.5f, -3.5f), vec3(0.0f), 0.0f, vec3(0.5f, 1.0f, 0.5f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], pillarText, norm_eff, object);
	Obj *pillar4 = new Obj(vec3(-3.0f, -1.5f, -3.5f), vec3(0.0f), 0.0f, vec3(0.5f, 1.0f, 0.5f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], pillarText, norm_eff, object);

	// Ontop of the platform are two walls, more pillars, point light and piece of glass
	Obj *platBox = new Obj(vec3(3.5, 1.0, 0.0), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["platBox"], &myScene->materials["platBox"], platText, blending, object);
	Obj *platWall = new Obj(vec3(-3.0, 1.5, 0.0), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["platWall"], &myScene->materials["platWall"], platText, shadowEff, object);
	Obj *glassPane = new Obj(vec3(-1.0, 1.0, 1.0), vec3(0.0f, 0.0f, 0.0f), 0.0, vec3(1.0f, 1.0f, 1.0f), &myScene->meshes["glass"], &myScene->materials["glass"], glassText, phongEff, glassOb);
	
	// loaded model
	Obj *stoneModel = new Obj(vec3(-3.0, 0.0, 3.0), vec3(0.0f), 0.0f, vec3(0.05f), &myScene->meshes["model"], &myScene->materials["model"], stoneModText, shadowEff, object);

	// bar geom ontop of pillars
	Obj *bar = new Obj(vec3(0.0, 2.5, 3.5), vec3(0.0), 0.0f, vec3(1.0, 0.5, 0.5), &myScene->meshes["bar"], &myScene->materials["platform"], platText, shadowEff, object);
	
	Obj *pillarPlat = new Obj(vec3(-2.5f, -2.75f, 0.0f), vec3(0.0f), 0.0f, vec3(0.5, 1.5f, 1.0f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], platText, shadowEff, object);
	Obj *pillarPlat2 = new Obj(vec3(-1.5f, -2.75f, 0.0f), vec3(0.0f), 0.0f, vec3(0.5f, 1.5f, 1.0f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], platText, shadowEff, object);
	Obj *pillarPlat3 = new Obj(vec3(-0.5f, -2.75f, 0.0f), vec3(0.0f), 0.0f, vec3(0.5f, 1.5f, 1.0f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], platText, shadowEff, object);
	Obj *pillarPlat4 = new Obj(vec3(0.5f, -2.75f, 0.0f), vec3(0.0f), 0.0f, vec3(0.5f, 1.5f, 1.0f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], platText, shadowEff, object);
	Obj *pillarPlat5 = new Obj(vec3(1.5f, -2.75f, -5.0f), vec3(0.0f), 0.0f, vec3(0.5f, 1.5f, 1.0f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], platText, shadowEff, object);
	Obj *pillarPlat6 = new Obj(vec3(2.5f, -2.75f, 0.0f), vec3(0.0f), 0.0f, vec3(0.5f, 1.5f, 1.0f), &myScene->meshes["cylinder"], &myScene->materials["cylinder"], platText, shadowEff, object);


	//pillarPlat->setTranslationParams(vec3(1.0, 0.0, 0.0), vec3(10.0, 0.0, 0.0));  (-0.20, 0.0, 0.5)
	Obj *spotlight = new Obj(vec3(-0.20, 0.0, 0.5), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(1.0, 1.0, 1.0), &myScene->meshes["spotlight"], &myScene->materials["spotlight"], woodenTextures, phongEff, spotty);
	// point light rotating hierarchy.
	Obj *pointLightParent = new Obj(vec3(5, 1, -8), vec3(0.0f, 1.0f, 0.0f), pi<float>(), vec3(0.1f), &myScene->meshes["pointLightParent"], &myScene->materials["pointLightYellow"], displacementTextures, displacement, pointLightObj);
	Obj *pointLightChildBall = new Obj(vec3(5.0f, 0.0, 0.0), vec3(1.0f, 0.0f, 0.0f), 2 * pi<float>(), vec3(1.0f), &myScene->meshes["pointLightParent"], &myScene->materials["pointLightBlue"], displacementTextures, displacement, pointLightObj);
	Obj *pointLightChildBall2 = new Obj(vec3(5.0f, 5.0, 0.0), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(1.0f), &myScene->meshes["pointLightParent"], &myScene->materials["pointLightRed"], displacementTextures, displacement, pointLightObj);

	// water for scene plane slightly higher than terrain.
	Obj *water = new Obj(vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f, vec3(5.0f, 5.0f, 5.0f), &myScene->meshes["water"], &myScene->materials["water"], waterText, water_eff, waterObj);

	// box and pyramid to show scale and hierarchy
	Obj *box = new Obj(vec3(1.0, 1.0, 2.0), vec3(0.0f), 0.0f, vec3(1.0f), &myScene->meshes["box"], &myScene->materials["box"], woodenTextures, norm_eff, movingObject);

	// set scale factor, sets the factor the sinwave is multiplied by and is applied to the box
	box->setScaleFactor(0.5f);

	// pyramid is the boxes child, so should scale as the box does through the hierarchy
	Obj *pyra = new Obj(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f), 0.0f, vec3(1.0f), &myScene->meshes["pyramid"], &myScene->materials["pyramid"], woodenTextures, norm_eff, object);
	

	// Set up of hierarchy/ scene graph ( terrrain 1 2, 3 4 as looking at the platform)
	// assign a parent to each object within the scene to ensure it renders.

	// terrain block 1:
	terrain1->addChild(platLower, "platformLower");

	// platform children
	platLower->addChild(platformUpper, "platformUpper");
	platformUpper->addChild(platWall, "platWall");
	platformUpper->addChild(platBox, "platBox");
	platformUpper->addChild(bar, "bar");
	bar->addChild(pillarPlat, "pillarPlat");
	bar->addChild(pillarPlat2, "pillarPlat2");
	bar->addChild(pillarPlat3, "pillarPlat3");
	bar->addChild(pillarPlat4, "pillarPlat4");
	bar->addChild(pillarPlat5, "pillarPlat5");
	bar->addChild(pillarPlat6, "pillarPlat6");
	platformUpper->addChild(glassPane, "glassPlane");
	platformUpper->addChild(stoneModel, "model");

	platBox->addChild(spotlight, "spotlight");
	

	platLower->addChild(pillar, "pillar");
	platLower->addChild(pillar2, "pillar2");
	platLower->addChild(pillar3, "pillar3");
	platLower->addChild(pillar4, "pillar4");



	// terrain block 3: contains the two spheres showing the difference between gouraud and phong shading
	terrain3->addChild(sphereG, "gouraudSphere");
	sphereG->addChild(sphereP, "phongSphere");
	
	terrain3->addChild(box, "box");
	box->addChild(pyra, "pyramid");

	// terrain block 4: contains point light hierarchy
	terrain4->addChild(pointLightParent, "pointLightParent");
	pointLightParent->addChild(pointLightChildBall, "pointChild");
	pointLightChildBall->addChild(pointLightChildBall2, "pointChild2");


	// push back all object pointers to list of objects
	myScene->list.push_back(water);
	myScene->list.push_back(box);
	myScene->list.push_back(pyra);
	myScene->list.push_back(pillar);
	myScene->list.push_back(pillar2);
	myScene->list.push_back(pillar3);
	myScene->list.push_back(pillar4);
	myScene->list.push_back(pointLightParent);
	myScene->list.push_back(platLower);
	myScene->list.push_back(platformUpper);
	myScene->list.push_back(platBox);
	myScene->list.push_back(platWall);
	myScene->list.push_back(pillarPlat);
	myScene->list.push_back(pillarPlat2);
	myScene->list.push_back(pillarPlat3);
	myScene->list.push_back(pillarPlat4);
	myScene->list.push_back(pillarPlat5);
	myScene->list.push_back(pillarPlat6);
	myScene->list.push_back(spotlight);
	myScene->list.push_back(sphereG);
	myScene->list.push_back(sphereP);
	myScene->list.push_back(glassPane);
	myScene->list.push_back(stoneModel);
	myScene->list.push_back(bar);
	myScene->list.push_back(pointLightChildBall);
	myScene->list.push_back(pointLightChildBall2);

	myScene->transparentObjects.push_back(glassPane);

	myScene->lightObjects.push_back(pointLightParent);
	myScene->lightObjects.push_back(pointLightChildBall);
	myScene->lightObjects.push_back(pointLightChildBall2);
	
	myScene->lightObjects.push_back(spotlight);


    // ******************************
    // Create box geometry for skybox
    // ******************************
	myScene->generator->generate_skybox(myScene->meshes["skybox"], myScene->cubemaps["outer"], true);  // outer is true inner is false for the bool to determine what textures to use
	myScene->generator->generate_skybox(myScene->meshes["skyboxInner"], myScene->cubemaps["inner"], false);
	
    // *********************
    // Load in skybox effect
    // *********************
	effect *sky_eff = new effect;
	sky_eff->add_shader("..\\resources\\shaders\\skybox.vert", GL_VERTEX_SHADER);
	sky_eff->add_shader("..\\resources\\shaders\\skybox.frag", GL_FRAGMENT_SHADER);

    // Build effect
    sky_eff->build();
	myScene->effectList.push_back(sky_eff);

	myScene->skybx = new Obj(vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(100.0f, 100.0f, 100.0f), &myScene->meshes["skybox"], myScene->cubemaps["outer"], sky_eff);
	myScene->skybx->setName("sky1");
	
	Obj *skybx2 = new Obj(vec3(0.0f, 1.0f, 0.0f), pi<float>(), vec3(90.0, 90.0, 90.0), &myScene->meshes["skyboxInner"], myScene->cubemaps["inner"], sky_eff);

	myScene->list.push_back(skybx2);
	myScene->skybx->addChild(skybx2, "skyInner");
	skybx2->addChild(water, "water");
	skybx2->addChild(terrain1, "terrain1");  
	skybx2->addChild(terrain2, "terrain2");
	skybx2->addChild(terrain3, "terrain3");
	skybx2->addChild(terrain4, "terrain4");
	myScene->list.push_back(myScene->skybx);



	// create radii effect with geometry shader
	effect *rad_eff = new effect;
	rad_eff->add_shader("..\\resources\\shaders\\radiusVert.vert", GL_VERTEX_SHADER);
	rad_eff->add_shader("..\\resources\\shaders\\radiusGeom.geom", GL_GEOMETRY_SHADER);
	rad_eff->add_shader("..\\resources\\shaders\\radiusFrag.frag", GL_FRAGMENT_SHADER);
	rad_eff->build();
	myScene->rad_eff = rad_eff;
	myScene->effectList.push_back(rad_eff);

	// add shaders to frustrum effect
	myScene->frustrumEffect.add_shader("..\\resources\\shaders\\simple.vert", GL_VERTEX_SHADER);
	myScene->frustrumEffect.add_shader("..\\resources\\shaders\\simple.frag", GL_FRAGMENT_SHADER);
	// Build effect
	myScene->frustrumEffect.build();

	// create a new shadow effect
	effect *shadow_effect = new effect;
	shadow_effect->add_shader("..\\resources\\shaders\\spot.vert", GL_VERTEX_SHADER);
	shadow_effect->add_shader("..\\resources\\shaders\\spot.frag", GL_FRAGMENT_SHADER);
	shadow_effect->build();
	myScene->shad_eff = shadow_effect;
	myScene->effectList.push_back(shadow_effect);


	// initialise screenquad and build postprocessing effects
	myScene->initQuad();

    return true;
}

bool update(float delta_time)
{
	myScene->updateScene(delta_time);
    return true;
}

bool render()
{
	myScene->renderScene();
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