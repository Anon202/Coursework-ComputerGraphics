#include "SceneManager.h"
#include "Obj.h"
#include "bloom.h"

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
	/*bloomEffect.add_shader("..\\resources\\shaders\\bloom\\bloom.vert", GL_VERTEX_SHADER);
	bloomEffect.add_shader("..\\resources\\shaders\\bloom\\bloom.frag", GL_FRAGMENT_SHADER);
	bloomEffect.build();		*/				 
												  
	lightEffect.add_shader("..\\resources\\shaders\\bloom\\bloom.vert", GL_VERTEX_SHADER);
	lightEffect.add_shader("..\\resources\\shaders\\bloom\\light.frag", GL_FRAGMENT_SHADER);
	lightEffect.build();						  
												
	blurEffect.add_shader("..\\resources\\shaders\\bloom\\blurBloom.vert", GL_VERTEX_SHADER);
	blurEffect.add_shader("..\\resources\\shaders\\bloom\\blurBloom.frag", GL_FRAGMENT_SHADER);
	blurEffect.build();							 
												  
	bloomFinal.add_shader("..\\resources\\shaders\\bloom\\bloomFinal.vert", GL_VERTEX_SHADER);
	bloomFinal.add_shader("..\\resources\\shaders\\bloom\\bloomFinal.frag", GL_FRAGMENT_SHADER);
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
	extern SceneManager* myScene;
	camera* cam = myScene->cam;


	// first pass render HDR buffer
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myScene->skybx->render();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// render check
	//renderer::set_render_target();

	//renderer::bind(*myScene->getBlurEffect());

	//glUniformMatrix4fv(
	//	myScene->getBlurEffect()->get_uniform_location("MVP"),
	//	1,
	//	GL_FALSE,
	//	value_ptr(mat4(1.0f)));


	//// Bind texture
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);


	//glUniform1i(myScene->getBlurEffect()->get_uniform_location("tex"), 0);

	//vec2 res = vec2(renderer::get_screen_width(), renderer::get_screen_height());
	//glUniform2f(myScene->getBlurEffect()->get_uniform_location("resolution"), res.x, res.y);

	//renderer::render(myScene->getScreenQuad());

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);


	/////////////////////
	//renderer::set_render_target();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//renderer::bind(*myScene->getBlurEffect());

	//glUniformMatrix4fv(
	//	myScene->getBlurEffect()->get_uniform_location("MVP"),
	//	1,
	//	GL_FALSE,
	//	value_ptr(mat4(1.0f)));


	//// Bind texture
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[0]);


	//glUniform1i(myScene->getBlurEffect()->get_uniform_location("tex"), 0);
	//vec2 res = vec2(renderer::get_screen_width(), renderer::get_screen_height());
	//glUniform2f(myScene->getBlurEffect()->get_uniform_location("resolution"), res.x, res.y);

	//renderer::render(myScene->getScreenQuad());

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render bright to ping
	///////////////////////////////////////
	//glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[0]);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//renderer::bind(*myScene->getBlurEffect());

	//glUniformMatrix4fv(
	//	myScene->getBlurEffect()->get_uniform_location("MVP"),
	//	1,
	//	GL_FALSE,
	//	value_ptr(mat4(1.0f)));


	//// Bind texture HDR brightness
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);


	//glUniform1i(myScene->getBlurEffect()->get_uniform_location("tex"), 0);
	//
	//vec2 res = vec2(renderer::get_screen_width(), renderer::get_screen_height());
	//glUniform2f(myScene->getBlurEffect()->get_uniform_location("resolution"), res.x, res.y);

	//renderer::render(myScene->getScreenQuad());
	//

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);





	// LAST PASS

	renderer::set_render_target();
	renderer::bind(bloomFinal);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongFBO[0]);/*
	glUniform1i(bloomFinal.get_uniform_location("bloom"), true);
	glUniform1f(bloomFinal.get_uniform_location("exposure"), 1.0f);*/
	renderer::render(myScene->getScreenQuad());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	

	//// 2. Blur bright fragments w/ two-pass Gaussian Blur 
	//GLboolean horizontal = true, first_iteration = true;
	//GLuint amount = 10;
	//renderer::bind(blurEffect);
	//for (GLuint i = 0; i < amount; i++)
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
	//	glUniform1i(blurEffect.get_uniform_location("horizontal"), horizontal);
	//	glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
	//	renderer::render(myScene->getScreenQuad());
	//	horizontal = !horizontal;
	//	if (first_iteration)
	//		first_iteration = false;

	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// 2. Now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//renderer::set_render_target();

	//renderer::bind(bloomFinal);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[0]);/*
	//glUniform1i(bloomFinal.get_uniform_location("bloom"), true);
	//glUniform1f(bloomFinal.get_uniform_location("exposure"), 1.0f);*/
	//renderer::render(myScene->getScreenQuad());


	//// Swap the buffers
	//glfwSwapBuffers(renderer::get_window());


}