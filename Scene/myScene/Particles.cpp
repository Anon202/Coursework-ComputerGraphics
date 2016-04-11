#include "Particles.h"

// Maximum number of particles
const unsigned int MAX_PARTICLES = 100;

// Particles in the system
static particle particles[MAX_PARTICLES];

// The transform feedback buffers
static GLuint transform_feedbacks[2];
// The particle buffers
static GLuint particle_buffers[2];

// Effects
effect eff;
effect particle_eff;

texture tex;

// Current buffer to perform the physics update to
static unsigned int front_buf = 0;
// Buffer to render to
static unsigned int back_buf = 1;

extern SceneManager* myScene;

static GLuint m_vao[2];

// method for initialising the number of particles
void initialiseParticles(int numofParticles)
{
	default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
	uniform_real_distribution<float> dist;

	// ******************************************
	// Generate transform feedback buffers on GPU
	// ******************************************
	glGenTransformFeedbacks(2, transform_feedbacks);

	// ********************************
	// Generate particle buffers on GPU
	// ********************************
	glGenBuffers(2, particle_buffers);


	// *******************
	// Initilise particles
	// *******************
	for (unsigned int i = 0; i < MAX_PARTICLES; ++i)
	{
		float fi = (((float)i) / ((float)MAX_PARTICLES)) * 2.0f* pi<float>();
		particles[i].velocity = vec3(dist(rand)*fi, 2.0f, 0.0f);
		particles[i].position = vec3(-350.0f, 0.0f, 0.0f);
		float life = (dist(rand)* 10.0f) + 2.0f;
		particles[i].lifetime = vec2(0.0f, life);
	}

	// ************************
	// Allocate the two buffers
	// ************************
	for (unsigned int i = 0; i < 2; ++i)
	{
		// if the vao has not been generated
		if (m_vao[i] == 0)
		{
			// generate and check for errors
			glGenVertexArrays(1, &m_vao[i]);
			assert(!CHECK_GL_ERROR && "Couldn't create VAO.");
		}

		// bind vao
		glBindVertexArray(m_vao[i]);
	
		// bind vbo for particle's movement
		glBindBuffer(GL_ARRAY_BUFFER, particle_buffers[i]);

		// bind data to buffer (particles)
		glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
		
		// set incoming value expectations
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)offsetof(particle, position));
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)offsetof(particle, velocity));
		glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)offsetof(particle, lifetime));
		glEnableVertexAttribArray(5); // pos location
		glEnableVertexAttribArray(6); // velocity location
		glEnableVertexAttribArray(7); // lifetime location
		
		// check if buffer was created
		assert(!CHECK_GL_ERROR && "Error creating Buffer with OpenGL");

		// bind transformfeedback buffer
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedbacks[i]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_buffers[i]);
		glBindVertexArray(0);
	}

	// Build effects
	eff.add_shader("..\\resources\\shaders\\smoke.vert", GL_VERTEX_SHADER);
	eff.add_shader("..\\resources\\shaders\\smoke.geom", GL_GEOMETRY_SHADER);
	eff.add_shader("..\\resources\\shaders\\smoke.frag", GL_FRAGMENT_SHADER);
	eff.build();
	

	particle_eff.add_shader("..\\resources\\shaders\\particle.vert", GL_VERTEX_SHADER);
	particle_eff.add_shader("..\\resources\\shaders\\particle.geom", GL_GEOMETRY_SHADER);
	particle_eff.add_shader("..\\resources\\shaders\\particle.frag", GL_FRAGMENT_SHADER);
	particle_eff.build();

	tex = texture("..\\resources\\textures\\smoke.png");

	// Use the particle effect
	renderer::bind(particle_eff);

	// **************************************
	// Tell OpenGL what the output looks like
	// **************************************
	const GLchar* attrib_names[3] =
	{
		"position_out",
		"velocity_out",
		"lifetime_out"
	};

	// set varying attributes (outgoing)
	glTransformFeedbackVaryings(particle_eff.get_program(), 3, attrib_names, GL_INTERLEAVED_ATTRIBS);

	// **************
	// Relink program
	// **************
	glLinkProgram(particle_eff.get_program());
	// check if buffer was created
	assert(!CHECK_GL_ERROR && "Error creating Buffer with OpenGL");

}


void updateParticles(float delta_time)
{
	static bool first_frame = true;

	// ********************
	// Bind particle effect
	// ********************
	renderer::bind(particle_eff);


	// **************************
	// Set the delta_time uniform
	// **************************
	glUniform1f(particle_eff.get_uniform_location("delta_time"), delta_time);

	// *******************************
	// Tell OpenGL we aren't rendering
	// *******************************
	glEnable(GL_RASTERIZER_DISCARD);
	
	// *******************************
	// Bind the buffers for use
	// - buffer is front buf
	// - transform feeback is back buf  
	glBindVertexArray(m_vao[front_buf]);
	glBindBuffer(GL_ARRAY_BUFFER, particle_buffers[front_buf]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedbacks[back_buf]);

	glBeginTransformFeedback(GL_POINTS);

	if (first_frame)
	{
		glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
		first_frame = false;
	}
	else
	{
		glDrawTransformFeedback(GL_POINTS, transform_feedbacks[front_buf]);
	}

	// End the transform feedback
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);
}


void renderParticles()
{
	// Bind the effect
	renderer::bind(eff);

	// Set the MVP matrix
	auto M = mat4(1.0f);
	auto V = myScene->cam->get_view();
	auto P = myScene->cam->get_projection();
	auto MV = V * M;
	glUniformMatrix4fv(
		eff.get_uniform_location("MV"),
		1,
		GL_FALSE,
		value_ptr(MV));
	glUniformMatrix4fv(
		eff.get_uniform_location("P"),
		1,
		GL_FALSE,
		value_ptr(P));

	// Set particle size
	glUniform1f(eff.get_uniform_location("point_size"), 10.0f);

	// bind texture and set uniform
	renderer::bind(tex, 0);
	glUniform1i(eff.get_uniform_location("tex"), 0);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind the back particle buffer for rendering
	glBindVertexArray(m_vao[back_buf]);
	glBindBuffer(GL_ARRAY_BUFFER, particle_buffers[back_buf]);

	// Describe the data we are interested in (just position) ** don't need to do this as using vao

	// Perform the render by drawing the transform feedback
	glDrawTransformFeedback(GL_POINTS, transform_feedbacks[back_buf]);


	// Disable vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);

	// Swap front and back buffers
	front_buf = back_buf;
	back_buf = (back_buf + 1) & 0x1;
	glPointSize(1.0f);
}