#include "game.h"

Game::Game() : m_width(1024), m_height(1280), m_box_r(10), m_max_velocity(0.3)
{
	createWindow(m_width, m_height, "game", glm::vec3(0.5, 0.5, 0.5));
	m_octree = new  OcTree(0, Cube(glm::vec3(0), m_box_r));
};
Game::~Game()
{
	delete m_octree;
	delete m_model;
	delete  m_material;
};

glm::vec3 scale(0.42f, 0.42, 0.42); // raduis 1

glm::vec3 lightPos(0, 0, 5);
void Game::init()
{
	glm::mat4 projection = glm::perspective(glm::radians(65.f), ((float)Window::getWidth() / (float)Window::getHeight()), 0.1f, 1000.0f);
	FPSCamera*	tps = new  FPSCamera(projection);
	setCamera(tps);

	m_material = new Material(new Shader("Shaders/BasicShader.vs", "Shaders/BasicShader.fs"));
	m_material->setUniform("lightPos", lightPos);
	m_model = Loader::loadModelFromFile("../Models/SphereHighPoly.obj");
	initparticles();
	// box 
	Entity * box_ent = new Entity(Loader::loadModelFromFile("../Models/cube.obj"), glm::translate(glm::mat4(), glm::vec3(0, 0, 0))* glm::scale(glm::mat4(), glm::vec3(m_box_r)));
	box_ent->setMaterial(m_material);
	box_ent->setMode(RenderMode::Line);
	AddEntity(box_ent);
};

void  Game::initparticles()
{
	for (int i = 0; i <200; i++)
	{
		glm::vec3 p, v;
		for (int i = 0; i < 3; i++) {

			p[i] = getRandom(m_box_r - 2, -m_box_r + 2);
		}
		if (pInp(p))
		{
			i--;
			continue;
		}
		for (int i = 0; i < 3; i++) {

			v[i] = getRandom(m_max_velocity, 0.1);
		}
		addParticle(p, v, 1);

	}


}

bool  Game::pInp(const glm::vec3& pos)
{
	for each (Particle* entity1 in m_entities)
	{
		glm::vec3 p = entity1->getPosition() - pos;
		float l = glm::length(p);
		if (l < 2)
		{
			return true;
		}
	}
	return false;
}



void Game::addParticle(const glm::vec3 & pos, const glm::vec3 & vel, float r)
{
	Entity * sphere_ent = new Entity(m_model, glm::translate(glm::mat4(), pos)* glm::scale(glm::mat4(), scale));
	sphere_ent->setMaterial(m_material);
	AddEntity(sphere_ent);
	Particle * p = new Particle(sphere_ent, vel);


	m_entities.push_back(p);
}

void prp(std::vector<Particle*>** list)
{
	*list = new std::vector<Particle*>();
	(*list)->push_back(nullptr);
}

void Game::update()
{

	for each (Particle* entity1 in m_entities)
	{
		checkBoxCollision(entity1);
	}

	m_octree->clear();

	for each (Particle* entity1 in m_entities)
	{
		m_octree->insert(entity1);
	}



	for each (Particle* entity1 in m_entities) {

		std::vector<Particle*>*particles=nullptr ;
		
     	m_octree->retrieve(&particles, entity1);

		for each (Particle* entity2 in *particles)
		{
			applyGPull(entity1, entity2);
			glm::vec3 p = entity1->getPosition() - entity2->getPosition();
			float l = glm::length(p);
			if (l < 2 && l != 0)
			{

				glm::vec3 tempa = (entity1->velocity*(entity1->mass - entity2->mass) + (2 * entity2->mass*entity2->velocity)) / (entity1->mass + entity2->mass);
				glm::vec3 tempb = (entity2->velocity*(entity2->mass - entity1->mass) + (2 * entity1->mass*entity1->velocity)) / (entity1->mass + entity2->mass);

				float d = (2 - l)*0.5;
				entity1->updatePos(glm::vec3(-d));
				entity2->updatePos(glm::vec3(d));
				entity1->velocity = tempa;
				entity2->velocity = tempb;
			}
		}
	}

	for (int i = 0; i < m_entities.size(); i++)
	{
		m_entities[i]->update();
	}

};


void  Game::applyGPull(Particle* p1, Particle* p2)
{
	float d = glm::distance(p1->getPosition(), p2->getPosition());
	if (d == 0) { return; }
	d = d*d;
	float f = G*(p1->mass* p2->mass) / d;
	float a1 = f / p1->mass;
	float a2 = f / p2->mass;

	p1->velocity += a1*DeltaT;
	p2->velocity += a2*DeltaT;
}


void  Game::checkBoxCollision(Particle * particle)
{
	const glm::vec3& pos = particle->getPosition();

	if ((pos.x > m_box_r))
	{
		particle->updatePos(glm::vec3(m_box_r - pos.x, 0, 0));

		particle->velocity.x = -particle->velocity.x;
	}
	else if (pos.x < -m_box_r)
	{
		particle->updatePos(glm::vec3(-m_box_r - pos.x, 0, 0));
		particle->velocity.x = -particle->velocity.x;

	}
	if ((pos.y > m_box_r))
	{
		particle->updatePos(glm::vec3(0, m_box_r - pos.y, 0));
		particle->velocity.y = -particle->velocity.y;

	}
	else if (pos.y < -m_box_r)
	{
		particle->updatePos(glm::vec3(0, -m_box_r - pos.y, 0));
		particle->velocity.y = -particle->velocity.y;
	}
	if ((pos.z > m_box_r))
	{
		particle->updatePos(glm::vec3(0, 0, m_box_r - pos.z));
		particle->velocity.z = -particle->velocity.z;

	}
	else if (pos.z < -m_box_r)
	{
		particle->updatePos(glm::vec3(0, 0, -m_box_r - pos.z));
		particle->velocity.z = -particle->velocity.z;

	}
}




void Game::tick()
{
	Scene::printFpsUps();
};


void Game::onRender()
{

}


int main()
{
	Game game;

	game.start();
	return 0;
}