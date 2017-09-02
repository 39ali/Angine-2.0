#include "game.h"

Game::Game() : m_bh1(3, 0, 0, 1), m_width(1024), m_height(1280), m_speed(90)
{
	createWindow(m_width, m_height, "game", true, glm::vec3(0, 0, 0));
};
Game::~Game()
{
	delete m_shader;
	delete m_computeShader;
};

glm::vec2 Game::getNormalizedDeviceCoords(double& x, double& y)
{
	float _x = (2 * x) / m_width - 1;
	float _y = (2 * y) / m_height - 1;

	return glm::vec2(_x, -_y);
}

void Game::init()
{
	glm::mat4 projection = glm::perspective(glm::radians(170.f), ((float)Window::getWidth() / (float)Window::getHeight()), 0.1f, 1000.0f);
	FPSCamera*	tps = new  FPSCamera(glm::radians(170.f), ((float)Window::getWidth() / (float)Window::getHeight()), 0.1f, 1000.0f);
	setCamera(tps);

	m_shader = new Shader("Shaders/particle.vs", "Shaders/particle.fs");
	m_computeShader = new Shader("Shaders/particle.cs");
	m_nParticles = glm::ivec3(200, 200, 200);
	particlesSum = m_nParticles.x * m_nParticles.y * m_nParticles.z;
	initbuffers();

	m_shader->use();
	//glm::mat4 projection =glm::perspective(glm::radians(50.0f), (float)m_width / m_height, 0.1f, 100.0f);
	m_shader->setUniform("projection", tps->getPorjection());
	glm::mat4 view = glm::lookAt(glm::vec3(2, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_shader->setUniform("view", view);

	m_shader->unuse();

};
void Game::update()
{
	if (Window::isKeyPressed(GLFW_KEY_W))
	{
		m_bh1.y += m_speed * Time::getDeltaTime();
	}
	if (Window::isKeyPressed(GLFW_KEY_S))
	{
		m_bh1.y -= m_speed* Time::getDeltaTime();
	}
	if (Window::isKeyPressed(GLFW_KEY_D))
	{
		m_bh1.x += m_speed * Time::getDeltaTime();
	}
	if (Window::isKeyPressed(GLFW_KEY_A))
	{
		m_bh1.x -= m_speed * Time::getDeltaTime();
	}

	if (Window::getInstance()->isKeyPressed(GLFW_KEY_ESCAPE))
	{
		Window::getInstance()->Close();
	}

};

void Game::tick()
{
	Scene::printFpsUps();
};
void  Game::initbuffers()
{
	// Initial positions of the particles
	std::vector<glm::vec4> initPos;
	std::vector<GLfloat> initVel(particlesSum * 4, 0.0f);
	glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
	GLfloat dx = 2.0f / (m_nParticles.x - 1),
		dy = 2.0f / (m_nParticles.y - 1),
		dz = 2.0f / (m_nParticles.z - 1);
	// We want to center the particles at (0,0,0)
	glm::mat4 transf = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -1, -1));
	for (int i = 0; i < m_nParticles.x; i++) {
		for (int j = 0; j < m_nParticles.y; j++) {
			for (int k = 0; k < m_nParticles.z; k++) {
				p.x = dx * i;
				p.y = dy * j;
				p.z = dz * k;
				p.w = 1.0f;// this one is just for padding
				p = transf * p;
				initPos.push_back(p);
			}
		}
	}

	GLuint bufs[2];
	glGenBuffers(2, bufs);
	GLuint posBuf = bufs[0];
	GLuint velBuf = bufs[1];

	GLuint bufSize = particlesSum * 4 * sizeof(GLfloat);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, initPos.data(), GL_DYNAMIC_DRAW);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, initVel.data(), GL_DYNAMIC_COPY);


	glGenVertexArrays(1, &m_Pvao);
	glBindVertexArray(m_Pvao);

	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// vel
	glBindBuffer(GL_ARRAY_BUFFER, velBuf);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// "black holes"
	glGenBuffers(1, &m_bhBuf);
	glBindBuffer(GL_ARRAY_BUFFER, m_bhBuf);
	GLfloat data[] = { m_bh1.x, m_bh1.y, m_bh1.z, m_bh1.w, };
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &m_bhVao);
	glBindVertexArray(m_bhVao);

	glBindBuffer(GL_ARRAY_BUFFER, m_bhBuf);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


}

void Game::onRender()
{
	//glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)Time::getTime()*glm::radians(20.f), glm::vec3(0, 0, 1.f));
	m_BlackHolePos = glm::vec3(m_bh1);

	m_computeShader->use();

	m_computeShader->setUniform("BlackHolePos", m_BlackHolePos);
	m_computeShader->setUniform("Gravity", 1000.f); //*(float)glm::tan(Time::getTime())

	glDispatchCompute(particlesSum / 1000, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// Draw the scene
	m_shader->use();

	glm::mat4  model = glm::mat4(1.0f);

	glPointSize(1.0f);

	m_shader->setUniform("model", model);
	m_shader->setUniform("Pcolor", glm::vec4(0, 0, 0, 0.2f));

	glBindVertexArray(m_Pvao);
	glDrawArrays(GL_POINTS, 0, particlesSum);
	glBindVertexArray(0);

	//draw black hole
	glPointSize(5.0f);
	GLfloat data[] = { m_BlackHolePos.x, m_BlackHolePos.y, m_BlackHolePos.z, 1.0f };
	glBindBuffer(GL_ARRAY_BUFFER, m_bhBuf);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(GLfloat), data);
	glBindVertexArray(m_bhVao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}


int main()
{
	Game game;

	game.start();
	return 0;
}