#pragma once
#include "Particle.h"
#include "Constraint.h"
#define STATIC_GLEW
#include <glew\glew.h>
class Cloth
{
public:
	Cloth() {};
	Cloth(float width, float height, int width_particles, int height_particlesd);
	~Cloth() {};
	void draw();
	void AddWindForce(const glm::vec3& direction);
	void update();

private:

	int m_width_particles;
	int m_height_particles;
	std::vector<Particle> m_particles;
	std::vector<Constraint> m_constraints;
	std::vector<glm::vec3>m_vertices;
	std::vector<glm::vec3>m_normals;
	std::vector<glm::vec2> m_textureCoords;
	std::vector<unsigned int > m_indices;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_normalVo;

private:
	Particle* getParticle(const int x, const int y) { return &m_particles[y*m_width_particles + x]; }
	void makeConstraint(Particle *p1, Particle *p2) { m_constraints.push_back(Constraint(p1, p2)); }
	void init();

	void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3&direction);
	glm::vec3 calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);
	void bindBufferData();
	void calcNewNormals();
};