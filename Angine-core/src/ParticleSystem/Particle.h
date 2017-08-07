#pragma once
#include <vector>
#include <glm\glm.hpp>

#define TIME_STEPSIZE2 0.5*0.5 // how large time step each particle takes each frame
#define DAMPING 0.01 // how much to damp the cloth simulation each frame

class Particle
{
public:
	Particle() 
	{};
	Particle(const glm::vec3 pos) :m_position(pos), m_old_pos(pos), m_movable(true), m_acceleration(glm::vec3(0, 0, 0)), m_mass(1)
		, m_normal(glm::vec3(0, 0, 0)) {}
	~Particle() {}
	void addForce(const glm::vec3 & force) 
	{ m_acceleration += force / m_mass; }

	//Given the equation "force = mass * acceleration" the next position is found through verlet integration* /
	void timeStep()
	{
		if (m_movable)
		{
			glm::vec3 temp = m_position;
			m_position = m_position + (m_position - m_old_pos)*(float)(1.0 - DAMPING) + m_acceleration*(float)(TIME_STEPSIZE2);
			m_old_pos = temp;
			m_acceleration = glm::vec3(0, 0, 0);
		}
	}

	glm::vec3& getPos() { return m_position; }
	glm::vec3* getPosPointer() { return &m_position; }
	void resetAcceleration() { m_acceleration = glm::vec3(0, 0, 0); }

	void offsetPos(const glm::vec3& v) { if (m_movable) m_position += v; }

	void makeUnmovable() { m_movable = false; }

	void addToNormal(const glm::vec3& normal)
	{
		m_normal += glm::normalize(normal);
	}

	glm::vec3& getNormal() { return m_normal; }

	void resetNormal() { m_acceleration = glm::vec3(0, 0, 0); }

private:
	glm::vec3 m_position;
	glm::vec3 m_old_pos;
	bool m_movable;
	glm::vec3 m_acceleration;
	float m_mass;
	glm::vec3 m_normal;
};