#pragma once
#include <vector>
#include "Particles.h"
#include "Cube.h"
#define MAX_PARTICLES 10
#define MAX_LEVELS 5

class OcTree
{
public:
	OcTree(int level, Cube bound);
	OcTree() {};
	~OcTree();
	void insert(Particle* particle);
	void clear();
	void retrieve( std::vector<Particle*>** list, Particle* p);

private:
	int getIndex(Particle* particle);
	void split();
	int m_level;
	std::vector<Particle*> m_particles;
	std::vector<OcTree> m_nodes;
	Cube m_bound;
};