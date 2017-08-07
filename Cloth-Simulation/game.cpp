#include "game.h"
#include "Sphere.h"
Game::Game() :m_particlesNum(glm::vec2(40, 40)), m_clothSize(glm::vec2(4, 3))
{
	createWindow(1024, 1280, "game");
};
Game::~Game()
{
	delete m_shader, m_computeCloth, m_computeNormals;

};

#define PRIMITIVE_RESTART 0xffffff
void Game::init()
{
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(PRIMITIVE_RESTART);
	m_shader = new Shader("Shaders/cloth.vs", "Shaders/cloth.fs");
	m_computeCloth = new Shader("Shaders/cloth.cs");
	m_computeNormals = new Shader("Shaders/normal.cs");
	m_shader->use();
	glm::mat4 projection = glm::perspective(glm::radians(50.0f), (float)Window::getWidth() / Window::getHeight(), 1.0f, 100.0f);
	m_shader->setUniform("projection", projection);
	m_shader->unuse();

	m_computeCloth->use();
	float dx = m_clothSize.x / (m_particlesNum.x - 1);
	float dy = m_clothSize.y / (m_particlesNum.y - 1);
	m_computeCloth->setUniform("RestLengthHoriz", dx);
	m_computeCloth->setUniform("RestLengthVert", dy);
	m_computeCloth->setUniform("RestLengthDiag", sqrtf(dx * dx + dy * dy));
	initBuffers();
	Model* m = Loader::loadModelFromFile("Models/SphereHighPoly.obj");
	Entity* ent = new Entity(m, glm::scale(glm::mat4(), glm::vec3(0.5, 0.5, 0.5)));
	Shader * shader = new Shader("Shaders/BasicShader.vs", "Shaders/BasicShader.fs");
	Material * material = new Material(shader);
	ent->setMaterial(material);
	AddEntity(ent);
};
void Game::initBuffers()
{
	glm::mat4 transf = glm::translate(glm::mat4(1.0), glm::vec3(0, m_clothSize.y, 0));
	transf = glm::rotate(transf, glm::radians(-80.0f), glm::vec3(1, 0, 0));
	transf = glm::translate(transf, glm::vec3(0, -m_clothSize.y, 0));




	std::vector<glm::vec4> pos;
	std::vector<glm::vec4> vel(m_particlesNum.x*m_particlesNum.y, glm::vec4(0));
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int>indices;
	float dx = m_clothSize.x / (m_particlesNum.x - 1);
	float dy = m_clothSize.y / (m_particlesNum.y - 1);

	float ds = 1.0f / (m_particlesNum.x - 1);
	float dt = 1.0f / (m_particlesNum.y - 1);

	glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec2 texc;
	for (int y = 0; y < m_particlesNum.y; y++)
	{
		for (int x = 0; x < m_particlesNum.x; x++)
		{
			p.x = dx*x;
			p.y = dy*y;
			pos.push_back(transf*p);

			texc.x = x*ds;
			texc.y = dt*y;
			texCoords.push_back(texc);
		}
	}

	//indices
	for (int y = 0; y < m_particlesNum.y - 1; y++)
	{
		for (int x = 0; x < m_particlesNum.x; x++)
		{
			indices.push_back((y + 0)*m_particlesNum.x + x);
			indices.push_back((y + 1)*m_particlesNum.x + x);
		}
		indices.push_back(PRIMITIVE_RESTART);
	}
	m_elementNum = indices.size();


	GLuint bufs[7];
	glGenBuffers(7, bufs);
	m_pos[0] = bufs[0];
	m_pos[1] = bufs[1];
	m_vel[0] = bufs[2];
	m_vel[1] = bufs[3];
	m_normal = bufs[4];
	m_indices = bufs[5];
	m_texCoords = bufs[6];




	unsigned int size = sizeof(glm::vec4)*pos.size();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_pos[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, pos.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_pos[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, pos.data(), GL_DYNAMIC_DRAW);


	//vel
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_vel[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_vel[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_COPY);

	//normal 
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_normal);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_COPY);

	//indices
	glBindBuffer(GL_ARRAY_BUFFER, m_indices);
	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_COPY);


	// Texcoord
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoords);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);


	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_pos[0]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_normal);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_texCoords);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	glBindVertexArray(0);
}



void Game::onRender()
{
	m_computeCloth->use();
	for (int i = 0; i < 1000; i++)
	{
		glDispatchCompute(m_particlesNum.x / 10, m_particlesNum.y / 10, 1);//how many work groups
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		swap = 1 - swap;
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_pos[swap]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_pos[1 - swap]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_vel[swap]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_vel[1 - swap]);
	}

	m_computeNormals->use();
	glDispatchCompute(m_particlesNum.x / 10, m_particlesNum.y / 10, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	m_shader->use();
	glm::mat4 view = glm::lookAt(glm::vec3(9, 2, 5), glm::vec3(2, 1, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0f);
	m_shader->setUniform("view", view);
	m_shader->setUniform("model", model);
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLE_STRIP, m_elementNum, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Game::update()
{

};
void Game::tick()
{
	Scene::printFpsUps();
};


int main()
{
	Game game;

	game.start();
	return 0;
}