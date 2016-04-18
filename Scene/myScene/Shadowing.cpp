#include "Shadowing.h"
extern SceneManager *myScene;

void updateShadows()
{

	myScene->shadow.light_position = vec3(myScene->lightList[spot]->get_position());
	myScene->shadow.light_dir = myScene->lightList[spot]->get_direction();

	// Press z to save
	if (glfwGetKey(renderer::get_window(), 'Z') == GLFW_PRESS)
	{
		myScene->shadow.buffer->save("test.png");
	}
}

void renderShadows()
{
	

	// render shadow map.
	renderer::set_render_target(myScene->shadow);

	// **********************
	// Clear depth buffer bit
	// **********************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ****************************
	// Set render mode to cull face
	// ****************************
	glCullFace(GL_FRONT);

	// Bind shader
	renderer::bind(*myScene->shad_eff);

	// Create MVP matrix
	for (auto &o : myScene->list)
	{

		if (o->myType != sky && o->myType != terrn && o->getName() != "spotlight")
		{
			// View matrix taken from shadow map
			auto V = myScene->shadow.get_view();
			auto P = myScene->cam->get_projection();
			auto MVP = P * V * o->mworld;
			// Set MVP matrix uniform
			glUniformMatrix4fv(
				myScene->shad_eff->get_uniform_location("MVP"), // Location of uniform
				1, // Number of values - 1 mat4
				GL_FALSE, // Transpose the matrix?
				value_ptr(MVP)); // Pointer to matrix data

			renderer::render(*o->m);
		}
	}


	// Set render target back to the screen
	renderer::set_render_target();


	// Set cull face to back
	glCullFace(GL_BACK);

}