#pragma once
#include <Angine.h>

class Particle
{
public:
	Particle(Entity* ent, const glm::vec3 & vel) :m_entity(ent), velocity(vel), mass(1)
	{};
	void setPosition(const glm::vec3& pos)
	{
		m_entity->setPosition(pos);
	};
	void updatePos(const glm::vec3& pos)
	{
		m_entity->setPosition(m_entity->getPosition() + pos);
	}

	void update()
	{
		updatePos(velocity);
	}

	inline glm::vec3 getPosition() { return  m_entity->getPosition(); }

	glm::vec3  velocity;
	float mass;
private:
	Entity* m_entity;

};
