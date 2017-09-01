#include "game.h"

Game::Game() : m_width(1024), m_height(1280), m_box_r(50), m_velocity(10, 30), m_velocity_max(30), particlesNum(300)
{
	createWindow(m_width, m_height, "game", false, glm::vec3(0.5, 0.5, 0.5));
	m_octree = new  OcTree(0, Cube(glm::vec3(0), m_box_r));
};
Game::~Game()
{
	delete m_octree;
	delete m_model;
	delete  m_material;
};

glm::vec3 scale(1.f / 2.39200807, 1.f / 2.39200807, 1.f / 2.39200807); // raduis 1

glm::vec3 lightPos(0, 0, 5);
void Game::init()
{

	FPSCamera*	tps = new  FPSCamera(glm::radians(65.f), ((float)Window::getWidth() / (float)Window::getHeight()), 0.1f, 1000.0f, glm::vec3(0, 0, 380));
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
	for (int i = 0; i < particlesNum; i++)
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

			v[i] = getRandom(m_velocity.y, m_velocity.x);
		}
		float mass = getRandom(1, 0.001);
		float density = getRandom(m_density.y, m_density.x);
		addParticle(p, v, mass);

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

void Game::addParticle(const glm::vec3 & pos, const glm::vec3 & vel, const float mass)
{
	float r = glm::min(glm::pow(mass, 0.8f), m_box_r - 1);
	Entity * sphere_ent = new Entity(m_model, glm::translate(glm::mat4(), pos)* glm::scale(glm::mat4(), r*scale));
	sphere_ent->setMaterial(m_material);
	AddEntity(sphere_ent);
	Particle * p = new Particle(sphere_ent, vel);
	p->mass = mass;
	p->radius = r;
	m_entities.push_back(p);
}

void prp(std::vector<Particle*>** list)
{
	*list = new std::vector<Particle*>();
	(*list)->push_back(nullptr);
}

void Game::checkParticlesCollision(Particle * entity1)
{
	for each (Particle*  entity2 in *entity1->particles)
	{
		//Particle*  entity2 = m_entities[i];

		glm::vec3 p = entity1->getPosition() - entity2->getPosition();
		float l = glm::length(p);
		if (l < entity1->radius + entity2->radius && l != 0 && entity2->isActive)
		{
			//glm::vec3 tempa = (entity1->velocity*(entity1->mass - entity2->mass) + (2 * entity2->mass*entity2->velocity)) / (entity1->mass + entity2->mass);
			//glm::vec3 tempv= (entity1->velocity + entity1->velocity)
			addParticle(entity2->getPosition(), (entity1->velocity + entity2->velocity) / 2.0f, entity1->mass + entity2->mass);
			entity2->isActive = false;
			entity1->isActive = false;

		}
	}


}

void Game::update()
{
	Window::getInstance()->disableCursor(0);

	for each (Particle* entity1 in m_entities)
	{
		checkBoxCollision(entity1);

		if (entity1->velocity.x > m_velocity_max)
			entity1->velocity.x = m_velocity_max;
		if (entity1->velocity.y > m_velocity_max)
			entity1->velocity.y = m_velocity_max;
		if (entity1->velocity.z > m_velocity_max)
			entity1->velocity.z = m_velocity_max;
	}

	for (size_t i = 0; i < m_entities.size(); i++)
	{

		for (size_t j = i; j < m_entities.size(); j++)
		{
			if (m_entities[i]->getId() != m_entities[j]->getId())
			{
				applyGPull(m_entities[i], m_entities[j]);
			}
		}
	}


	m_octree->clear();
	for each (Particle* entity1 in m_entities)
	{
		m_octree->insert(entity1);
	}


	int s = m_entities.size();
	for (std::size_t i = 0; i < s; i++)
	{
		checkParticlesCollision(m_entities[i]);
	}

	for (int i = 0; i < m_entities.size();)
	{
		if (!m_entities[i]->isActive)
			DeleteParticle(m_entities[i], i);
		else
			i++;
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

	if ((pos.x + particle->radius > m_box_r))
	{
		particle->updatePos(glm::vec3(m_box_r - (pos.x + particle->radius), 0, 0));

		particle->velocity.x = -particle->velocity.x;
	}
	else if (pos.x - particle->radius < -m_box_r)
	{
		particle->updatePos(glm::vec3(-m_box_r - (pos.x - particle->radius), 0, 0));
		particle->velocity.x = -particle->velocity.x;

	}
	if ((pos.y + particle->radius > m_box_r))
	{
		particle->updatePos(glm::vec3(0, m_box_r - (pos.y + particle->radius), 0));
		particle->velocity.y = -particle->velocity.y;

	}
	else if (pos.y - particle->radius < -m_box_r)
	{
		particle->updatePos(glm::vec3(0, -m_box_r - (pos.y - particle->radius), 0));
		particle->velocity.y = -particle->velocity.y;
	}
	if ((pos.z + particle->radius > m_box_r))
	{
		particle->updatePos(glm::vec3(0, 0, m_box_r - (pos.z + particle->radius)));
		particle->velocity.z = -particle->velocity.z;

	}
	else if (pos.z - particle->radius < -m_box_r)
	{
		particle->updatePos(glm::vec3(0, 0, -m_box_r - (pos.z - particle->radius)));
		particle->velocity.z = -particle->velocity.z;

	}
}


void Game::DeleteParticle(Particle * particle, int i)
{
	m_entities.erase(m_entities.begin() + i);
	DeleteEntity(particle->getEntity());
	delete particle;
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