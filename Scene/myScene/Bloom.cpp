#include "SceneManager.h"

GLuint hdrFBO;
GLuint rboDepth;
GLuint colorBuffers[2];
GLuint pingpongFBO[2];
GLuint pingpongColorbuffers[2];

effect bloomEffect;
effect blurEffect;
effect bloomFinal;
effect lightEffect;

void initialiseBloom()
{

	// http://www.learnopengl.com/#!Advanced-Lighting/Bloom

	// build effects
	bloomEffect.add_shader("..\\resources\\shaders\\bloom.vert", GL_VERTEX_SHADER);
	bloomEffect.add_shader("..\\resources\\shaders\\bloom.frag", GL_FRAGMENT_SHADER);
	bloomEffect.build();

	lightEffect.add_shader("..\\resources\\shaders\\bloom.vert", GL_VERTEX_SHADER);
	lightEffect.add_shader("..\\resources\\shaders\\light.frag", GL_FRAGMENT_SHADER);
	lightEffect.build();

	blurEffect.add_shader("..\\resources\\shaders\\blurBloom.vert", GL_VERTEX_SHADER);
	blurEffect.add_shader("..\\resources\\shaders\\blurBloom.frag", GL_FRAGMENT_SHADER);
	blurEffect.build();

	bloomFinal.add_shader("..\\resources\\shaders\\bloomFinal.vert", GL_VERTEX_SHADER);
	bloomFinal.add_shader("..\\resources\\shaders\\bloomFinal.frag", GL_FRAGMENT_SHADER);
	bloomFinal.build();


	// enable depth test
	glEnable(GL_DEPTH_TEST);
	

	GLuint SCR_WIDTH = renderer::get_screen_width();
	GLuint SCR_HEIGHT = renderer::get_screen_height();

	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	
	glGenTextures(2, colorBuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
			);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
			);
	}

	// create depth buffer
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// Also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void renderBloom()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
	//glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 model;
	//shader.Use();
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	//
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, woodTexture);
	//// - set lighting uniforms
	//for (GLuint i = 0; i < lightPositions.size(); i++)
	//{
	//	glUniform3fv(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
	//	glUniform3fv(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
	//}
	//glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, &camera.Position[0]);
	//// - create one large cube that acts as the floor
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
	//model = glm::scale(model, glm::vec3(25.0f, 1.0f, 25.0f));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//RenderCube();
	//// - then create multiple cubes as the scenery
	//glBindTexture(GL_TEXTURE_2D, containerTexture);
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//RenderCube();
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//RenderCube();
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
	//model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	//model = glm::scale(model, glm::vec3(2.0));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//RenderCube();
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
	//model = glm::rotate(model, 23.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	//model = glm::scale(model, glm::vec3(2.5));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//RenderCube();
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
	//model = glm::rotate(model, 124.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	//model = glm::scale(model, glm::vec3(2.0));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//RenderCube();
	//RenderCube();
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//RenderCube();
	//// - finally show all the light sources as bright cubes
	//shaderLight.Use();
	//glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	//for (GLuint i = 0; i < lightPositions.size(); i++)
	//{
	//	model = glm::mat4();
	//	model = glm::translate(model, glm::vec3(lightPositions[i]));
	//	model = glm::scale(model, glm::vec3(0.5f));
	//	glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//	glUniform3fv(glGetUniformLocation(shaderLight.Program, "lightColor"), 1, &lightColors[i][0]);
	//	RenderCube();
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// 2. Blur bright fragments w/ two-pass Gaussian Blur 
	//GLboolean horizontal = true, first_iteration = true;
	//GLuint amount = 10;
	//shaderBlur.Use();
	//for (GLuint i = 0; i < amount; i++)
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
	//	glUniform1i(glGetUniformLocation(shaderBlur.Program, "horizontal"), horizontal);
	//	glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
	//	RenderQuad();
	//	horizontal = !horizontal;
	//	if (first_iteration)
	//		first_iteration = false;
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// 2. Now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//shaderBloomFinal.Use();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	//glUniform1i(glGetUniformLocation(shaderBloomFinal.Program, "bloom"), bloom);
	//glUniform1f(glGetUniformLocation(shaderBloomFinal.Program, "exposure"), exposure);
	//RenderQuad();


	//// Swap the buffers
	//glfwSwapBuffers(window);
}