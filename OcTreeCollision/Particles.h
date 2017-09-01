#pragma once
#include <Angine.h>

class Particle
{
public:
	Particle(Entity* ent, const glm::vec3 & vel) :m_entity(ent), velocity(vel), mass(1), radius(1), particles(nullptr)
	{
		isActive = true;
	};
	void setPosition(const glm::vec3& pos)
	{
		m_entity->setPosition(pos);
	};

	void setScale(const glm::vec3& scale)
	{
		m_entity->setScale(scale);
	};
	void updatePos(const glm::vec3& pos)
	{
		m_entity->setPosition(m_entity->getPosition() + pos);
	}

	void update()
	{
		updatePos(velocity*(float)Time::getDeltaTime());
	}

	inline glm::vec3 getPosition() { return  m_entity->getPosition(); }
	inline int  getId() { return m_entity->getid(); }
	inline Entity* getEntity() { return m_entity; }
public:
	std::vector<Particle*>* particles;
	glm::vec3  velocity;
	float mass;
	float radius;
	float density;
	bool isActive;
private:
	Entity* m_entity;
};
