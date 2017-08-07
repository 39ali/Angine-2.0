#include "Cloth.h"


Cloth::Cloth(float width, float height, int width_particles, int height_particles) :m_width_particles(width_particles), m_height_particles(height_particles)
{
	m_particles.resize(width_particles*height_particles);

	int count = m_width_particles* m_height_particles;
	m_vertices.resize(count);
	m_textureCoords.resize(count);
	m_normals.resize(count);
	m_indices.resize(6 * (m_width_particles - 1)*(m_height_particles - 1));


	for (int x = 0; x < m_width_particles; x++)
	{
		for (int y = 0; y < m_height_particles; y++)
		{
			glm::vec3 pos = glm::vec3(width*(x / (float)m_width_particles), -height*(y / (float)m_height_particles), 0);
			m_vertices[y*m_width_particles + x] = pos;
			m_particles[y*m_width_particles + x] = Particle(pos);
		}

	}


	int pointer = 0;
	for (int gz = 0; gz < m_height_particles - 1; gz++) {
		for (int gx = 0; gx < m_width_particles - 1; gx++) {
			int topLeft = (gz*m_width_particles) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*m_width_particles) + gx;
			int bottomRight = bottomLeft + 1;
			m_indices[pointer++] = topLeft;
			m_indices[pointer++] = bottomLeft;
			m_indices[pointer++] = topRight;
			m_indices[pointer++] = topRight;
			m_indices[pointer++] = bottomLeft;
			m_indices[pointer++] = bottomRight;
		}
	}

	// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
	for (int x = 0; x < width_particles; x++)
	{
		for (int y = 0; y < m_height_particles; y++)
		{
			if (x < width_particles - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y));
			if (y < height_particles - 1) makeConstraint(getParticle(x, y), getParticle(x, y + 1));
			if (x < width_particles - 1 && y < height_particles - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y + 1));
			if (x < width_particles - 1 && y < height_particles - 1) makeConstraint(getParticle(x + 1, y), getParticle(x, y + 1));
		}
	}


	// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
	for (int x = 0; x < width_particles; x++)
	{
		for (int y = 0; y < height_particles; y++)
		{
			if (x < width_particles - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y));
			if (y < height_particles - 2) makeConstraint(getParticle(x, y), getParticle(x, y + 2));
			if (x < width_particles - 2 && y < height_particles - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y + 2));
			if (x < width_particles - 2 && y < height_particles - 2) makeConstraint(getParticle(x + 2, y), getParticle(x, y + 2));
		}
	}


	// making the upper left most three and right most three particles unmovable
	for (int i = 0; i < 3; i++)
	{
		getParticle(0 + i, 0)->offsetPos(glm::vec3(0.5, 0.0, 0.0)); // 
		getParticle(0 + i, 0)->makeUnmovable();

		getParticle(0 + i, 0)->offsetPos(glm::vec3(-0.5, 0.0, 0.0)); // 
		getParticle(width_particles - 1 - i, 0)->makeUnmovable();
	}
	init();
}





void Cloth::init()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_normalVo);
	glBindVertexArray(m_vao);

	//vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size()* sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//normals
	glBindBuffer(GL_ARRAY_BUFFER, m_normalVo);
	glBufferData(GL_ARRAY_BUFFER, m_indices.size()* sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	

	glBindVertexArray(0);
}


void Cloth::bindBufferData()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		Particle& p =  m_particles[i];
		m_vertices[i] = m_particles[i].getPos();
		m_normals[i] = p.getNormal();
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size()* sizeof(glm::vec3), m_vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_normalVo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_normals.size()* sizeof(glm::vec3), m_normals.data());

}

void Cloth::draw()
{
	glBindVertexArray(m_vao);
	bindBufferData();
	//glDrawArrays(GL_TRIANGLES, 0, m_width_particles*m_height_particles*0.5f);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}




void Cloth::AddWindForce(const glm::vec3& direction)
{
	for (int x = 0; x < m_width_particles - 1; x++)
	{
		for (int y = 0; y < m_height_particles - 1; y++)
		{
			Particle * p1 = getParticle(x + 1, y);
			Particle * p2 = getParticle(x, y);
			Particle * p3 = getParticle(x, y + 1);
			addWindForcesForTriangle(p1, p2, p3, direction);
			addWindForcesForTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), direction);
		}
	}
}
void Cloth::addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3& direction)
{
	glm::vec3 normal = calcTriangleNormal(p1, p2, p3);
	glm::vec3 d = glm::normalize(normal);
	glm::vec3 force = normal*(glm::dot(d, direction));
	p1->addForce(force);
	p2->addForce(force);
	p3->addForce(force);
}

glm::vec3 Cloth::calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3)
{
	glm::vec3& pos1 = p1->getPos();
	glm::vec3& pos2 = p2->getPos();
	glm::vec3& pos3 = p3->getPos();

	glm::vec3 v1 = pos2 - pos1;
	glm::vec3& v2 = pos3 - pos1;

	return glm::cross(v1, v2);
}

void Cloth::calcNewNormals()
{

	for (int x = 0; x < m_width_particles - 1; x++)
	{
		for (int y = 0; y < m_width_particles - 1; y++)
		{
			Particle* p1 = getParticle(x, y);
			Particle* p2 = getParticle(x, y + 1);
			Particle* p3 = getParticle(x + 1, y);

			glm::vec3 normal = calcTriangleNormal(p1, p2, p3);
			p1->addToNormal(normal);
			p2->addToNormal(normal);
			p3->addToNormal(normal);

			p1 = getParticle(x + 1, y + 1);
			normal = calcTriangleNormal(p3, p2, p1);
			p1->addToNormal(normal);
			p2->addToNormal(normal);
			p3->addToNormal(normal);

		}
	}
}

#define CONSTRAINT_ITERATIONS 15
void Cloth::update()
{

	for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
	{
		for (int i = 0; i < m_constraints.size(); i++)
		{
			m_constraints[i].satisfyConstraint(); // satisfy constraint.
		}

	}

	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i].timeStep();
	}

}