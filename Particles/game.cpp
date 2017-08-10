#include "game.h"

Game::Game() : bh1(7,0, 0, 1), bh2(-7, 0, 0, 1), m_width(1024), m_height(1280)
{
	createWindow(m_width, m_height, "game", glm::vec3(0, 0, 0));
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
	glm::mat4 projection = glm::perspective(glm::radians(65.f), ((float)Window::getWidth() / (float)Window::getHeight()), 0.1f, 1000.0f);
	TPScamera*	tps = new  TPScamera(glm::vec3(0, 0, 0), projection);
	setCamera(tps);

	m_shader = new Shader("Shaders/particle.vs", "Shaders/particle.fs");
	m_computeShader = new Shader("Shaders/particle.cs");
	m_nParticles = glm::ivec3(100, 100, 100);
	particlesSum = m_nParticles.x * m_nParticles.y * m_nParticles.z;
	initbuffers();

	m_shader->use();
	//glm::mat4 projection =glm::perspective(glm::radians(50.0f), (float)m_width / m_height, 0.1f, 100.0f);
	m_shader->setUniform("projection", projection);
	glm::mat4 view = glm::lookAt(glm::vec3(2, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_shader->setUniform("view", view);

	m_shader->unuse();

};
void Game::update()
{

};
glm::vec3 mouse3dpos;
void Game::tick()
{

	
	//Scene::printFpsUps();
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
	GLfloat data[] = { bh1.x, bh1.y, bh1.z, bh1.w, bh2.x, bh2.y, bh2.z, bh2.w };
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &m_bhVao);
	glBindVertexArray(m_bhVao);

	glBindBuffer(GL_ARRAY_BUFFER, m_bhBuf);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


}

void Game::onRender()
{





	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)Time::getTime()*glm::radians(20.f), glm::vec3(glm::tan(Time::getDeltaTime())*0.7f, glm::cos(Time::getTime())*0.2f* 0.5f,1.f));
	glm::vec3 BlackHolePos1 = glm::vec3(rot*bh1);// mouse3dpos;
	glm::vec3 BlackHolePos2 = glm::vec3(rot*bh2);

	m_computeShader->use();
	m_computeShader->setUniform("BlackHolePos1", BlackHolePos1);
	m_computeShader->setUniform("BlackHolePos2", BlackHolePos2);
	m_computeShader->setUniform("Gravity2", 1000.f*(float)glm::tan(Time::getTime()));
	glDispatchCompute(particlesSum / 1000, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// Draw the scene
	m_shader->use();


	glm::mat4  model = glm::mat4(1.0f);

	glPointSize(1.0f);

	m_shader->setUniform("model", model);
	m_shader->setUniform("Pcolor", glm::vec4(0, 0, 0, 0.2f));
	m_shader->setUniform("BlackHolePos1", BlackHolePos1);
	m_shader->setUniform("BlackHolePos2", BlackHolePos2);
	glBindVertexArray(m_Pvao);
	glDrawArrays(GL_POINTS, 0, particlesSum);
	glBindVertexArray(0);


	//draw black holes
	glPointSize(5.0f);
	GLfloat data[] = { BlackHolePos1.x, BlackHolePos1.y, BlackHolePos1.z, 1.0f, BlackHolePos2.x, BlackHolePos2.y, BlackHolePos2.z, 1.0f };
	glBindBuffer(GL_ARRAY_BUFFER, m_bhBuf);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(GLfloat), data);
	m_shader->setUniform("Pcolor", glm::vec4(1, 1, 0, 1.0f));
	glBindVertexArray(m_bhVao);
	glDrawArrays(GL_POINTS, 0, 2);
	glBindVertexArray(0);
}


int main()
{
	Game game;

	game.start();
	return 0;
}